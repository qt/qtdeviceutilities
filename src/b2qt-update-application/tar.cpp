/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://qt.digia.com/
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://qt.digia.com/
**
****************************************************************************/

#include "tar.h"
#include <QProcess>
#include <QDebug>
#include <openssl/x509.h>
#include <openssl/pem.h>

static bool isZeroed(const char *ptr, unsigned int size)
{
    while (size) {
        if (*ptr) {
            return false;
        }
        ++ptr;
        --size;
    }
    return true;
}

Tar::Tar(QIODevice *source)
    : QObject(source)
    , mSource(source)
    , mSize(0)
    , mRemainingSize(0)
    , mRemainingFileBytes(0)
    , mProcess(0)
    , mState(WaitForHeader)
    , x509(0)
    , mdctx(0)
{
    // Initialize table with all digests in order to look them up by string later
    OpenSSL_add_all_digests();

    connect(source, &QIODevice::readyRead, this, &Tar::dataIncoming);
    connect(source, &QIODevice::aboutToClose, this, &Tar::aboutToClose);
}

Tar::~Tar()
{
    if (x509)
        X509_free(x509);
    x509 = 0;
    if (mdctx)
        EVP_MD_CTX_destroy(mdctx);
    mdctx = 0;
}

const QByteArray &Tar::currentContent() const
{
    return mContent;
}

unsigned long Tar::currentSize() const
{
    return mSize;
}

QString Tar::currentFileName() const
{
    if (mHeader.name[sizeof(mHeader.name)-1])
        return QString::fromLatin1(mHeader.name, sizeof(mHeader.name));
    else
        return QString::fromLatin1(mHeader.name);
}

void Tar::dataIncoming()
{
    while (mSource->bytesAvailable() > 512) {

    if (mState == WaitForHeader) {
        if (mSource->bytesAvailable() < 512)
            return;
        if (mSource->read((char*)&mHeader, sizeof(mHeader)) != sizeof(mHeader))
            qFatal("Tar read error");
        if (isZeroed((char*)&mHeader, sizeof(mHeader))) {
            qDebug() << "Zero Tar block";
            qDebug() << "Bytes left" << mSource->bytesAvailable();
            mSource->close();
            return;
        }
        if (mHeader.name[0] == 0)
            qFatal("Header starts with 0");

        mSize = QString::fromLatin1(mHeader.size, sizeof(mHeader.size)-1).toULongLong(0, 8);
        mRemainingFileBytes = mSize;
        mRemainingSize = mSize;
        if (mSize % 512) // padding to 512 byte block
          mRemainingSize += 512 - (mSize % 512);

        if (mSize == 0) {
            qFatal("Size of file is 0");
        }

        mState = WaitForDecision;
        qDebug() << "Started file" << currentFileName() << mSize;
        if (x509)
            setupMDContext();
        emit startingFile(currentFileName());
    } else if (mState == WaitForDecision) {
        return;
    } else if (mState == Extract) {
        QByteArray ba;

        quint64 bytesToHandle = mRemainingSize;

        if (mSource->bytesAvailable() < mRemainingSize)
            bytesToHandle = mSource->bytesAvailable();
        if (bytesToHandle > 60000)
            bytesToHandle = 60000;
        ba = mSource->read(bytesToHandle);

        updateMDContext(ba);
        mProcess->write(ba);
        mProcess->waitForBytesWritten();
        mRemainingSize -= ba.size();
        // do not return
    } else if (mState == Receive) {
        QByteArray ba;

        if (mSource->bytesAvailable() > mRemainingSize)
            ba = mSource->read(mRemainingSize);
        else
            ba = mSource->readAll();
        updateMDContext(ba);
        mContent += ba;
        mRemainingSize -= ba.size();
        if (mContent.size() > mSize)
            mContent.resize(mSize);
        // do not return
    } else if (mState == Skip) {
        QByteArray ba;

        if (mSource->bytesAvailable() > mRemainingSize) {
            ba = mSource->read(mRemainingSize);
        } else {
            ba = mSource->readAll();
        }
        updateMDContext(ba);
        mRemainingSize -= ba.size();
        // drop data
        // do not return
    } else if (mState == WaitForContinue) {
        return;
    } else {
        qFatal("Unknown tar state");
    }

    if (mState != WaitForContinue && mState != WaitForHeader && mRemainingSize == 0) {
        if (mState == Extract) {
            qDebug() << "End tar process";
            // please tar
            // it needs 2 zeroed blocks
            char buffer[sizeof(mHeader)];
            memset(buffer, 0, sizeof(buffer));
            mProcess->write(buffer, sizeof(buffer));
            mProcess->write(buffer, sizeof(buffer));
            mProcess->waitForBytesWritten();
            mProcess->closeWriteChannel();
            mProcess->waitForFinished(-1);
            mProcess->deleteLater();
            mProcess = 0;
        }
        mState = WaitForContinue;

        emit(endingFile(currentFileName()));
    }

    }
}

void Tar::extractContent(const QString &targetDir)
{
    if (mState == WaitForDecision) {
        qDebug() << Q_FUNC_INFO;
        mState = Extract;
        delete mProcess;
        mProcess = new QProcess(this);
        mProcess->setProcessChannelMode(QProcess::ForwardedChannels);
        mProcess->start("tar", QStringList() << "xvf" << "-" << "-C" << targetDir);

        // if we want to extract a file directly, the header needs to be passed to tar
        if (!currentFileName().endsWith(".tar"))
            mProcess->write((const char*)&mHeader, sizeof(mHeader));
    }
}

void Tar::receiveContent()
{
    if (mState == WaitForDecision)
        mState = Receive;
}

void Tar::skipContent()
{
    if (mState == WaitForDecision)
        mState = Skip;
}

void Tar::continueContent()
{
    if (mState == WaitForContinue) {
        mState = WaitForHeader;
        mSize = 0;
        mContent.clear();
        delete mProcess;
        mProcess = 0;
    } else
        qDebug() << "Called" << Q_FUNC_INFO << "in wrong state";
}

void Tar::aboutToClose()
{
    if (mState != WaitForHeader) {
        qWarning() << "unexpected close";
    }
    emit finished();
}

bool Tar::setVerificationData(const QString &certificateFileName, const QString &mdType)
{
    if (x509)
        qFatal("Setting key twice");

    // Load key from file
    BIO *i = BIO_new(BIO_s_file());
    BIO *o = BIO_new_fp(stdout,BIO_NOCLOSE);

    if ((BIO_read_filename(i, certificateFileName.toLatin1().constData()) <= 0) ||
        ((x509 = PEM_read_bio_X509_AUX(i, NULL, NULL, NULL)) == NULL)) {
        qWarning() << "Invalid certificate, failed to read file" << certificateFileName;
        return false;
    }
    // Showing used certificate
    X509_print_ex(o, x509, XN_FLAG_COMPAT, X509_FLAG_COMPAT);
    BIO_free(i);
    BIO_free(o);
    qDebug() << "Using verification key:" << certificateFileName;

    md = EVP_get_digestbyname(mdType.toLatin1().constData());
    if (md == NULL)
        qFatal("Digest not found");
    return true;
}

void Tar::setupMDContext()
{
    if (mdctx)
        EVP_MD_CTX_destroy(mdctx);
    if (!(mdctx = EVP_MD_CTX_create()))
        qFatal("EVP_MD_CTX_create failed");

    EVP_PKEY *key = X509_get_pubkey(x509);
    if (key == NULL) {
        qFatal("X509_get_pubkey failed");
    }

    if (md == NULL)
        qFatal("No digest set");

    if (1 != EVP_DigestVerifyInit(mdctx, NULL, md, NULL, key))
        qFatal("EVP_DigestVerifyInit failed");
}

void Tar::updateMDContext(QByteArray data)
{
    if (mRemainingFileBytes == 0)
        qFatal("Trying to update MD context when no more bytes are expected");

    if (mRemainingFileBytes < data.size())
        data = data.left(mRemainingFileBytes);
    mRemainingFileBytes -= data.size();

    if (!mdctx)
        return;

    if (1 != EVP_DigestVerifyUpdate(mdctx, data.constData(), data.size()))
          qFatal("EVP_DigestVerifyUpdate failed");
}

bool Tar::checkSignature(const QByteArray &signature)
{
    return 1 == EVP_DigestVerifyFinal(mdctx, (unsigned char*)signature.constData(), signature.size());
}

bool Tar::verifyCurrentContent(const QByteArray &signature)
{
    setupMDContext();
    if (1 != EVP_DigestVerifyUpdate(mdctx, mContent.constData(), mContent.size()))
          qFatal("EVP_DigestVerifyUpdate failed");
    return checkSignature(signature);
}
