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

#include "update.h"
#include "tar.h"
#include <QDebug>
#include <QCoreApplication>
#include <QProcess>
#include <QFile>

extern QStringList mounts;
bool writeAll(const QString &fileName, const QByteArray &content);
static QStringList allowedFiles;
QByteArray readAll(const QString &fileName);
bool execute(const QString &binary, const QStringList &arguments);
bool mount(const QString &device, const QString &mountpoint, const QString &arguments = QString());

QMap<QString,QString> parseMetaInfo(const QByteArray &data)
{
    QMap<QString,QString> rc;
    QList<QByteArray> list = data.split('\n');
    foreach (const QByteArray &ba, list) {
        if (ba.isEmpty())
            continue;
        int i = ba.indexOf(':');
        if (i < 0 || i == ba.size())
            qFatal("Invalid meta.info content: No colon found or no data after colon");

        QString key = ba.left(i);
        QString value = ba.mid(i+1).trimmed();

        if (rc.contains(key))
            qFatal("Invalid meta.info content: Same key given twice");
        rc.insert(key, value);
    }
    if (rc.isEmpty())
        qFatal("Invalid meta.info content: No keys given");
    return rc;
}

Update::Update(QObject *parent)
    : QObject(parent)
    , mTar(0)
    , mSource(0)
{
    allowedFiles << "key.info" << "meta.info" << "keys.tar" << "uImage" << "rootfs.tar";
}

Update::~Update()
{
}

//# openssl dgst -sha256 -sign priv.pem file.txt > file.txt.sig
//# openssl dgst -sha256 -verify public.pem -signature file.txt.sig file.txt

void Update::setDevice(QIODevice *source)
{
    if (!source) {
        qFatal("Source device is NULL");
        return;
    }

    if (mSource) {
        qFatal("Source already set");
        return;
    }

    mSource = source;
    mTar = new Tar(mSource);

    connect(mTar, &Tar::startingFile, this, &Update::tarStartingFile);
    connect(mTar, &Tar::endingFile, this, &Update::tarEndingFile);
    connect(mTar, &Tar::finished, this, &Update::tarFinished);
}

void Update::tarStartingFile(const QString &name)
{
    if (name == "key.info") {
            mTar->receiveContent();
    } else if (name.endsWith(".sig")) {
        mTar->receiveContent();
        mCurrentFile = name;
        mCurrentFile.chop(4);

        bool found = false;
        while (!allowedFiles.isEmpty()) {
            QString n = allowedFiles.takeFirst();
            if (n == mCurrentFile) {
                found = true;
                break;
            }
        }

        if (!found)
            qFatal("File not allowed");

    } else if (mCurrentFile != name) {
        qFatal("Invalid file order");
    } else {
        if (mCurrentFile == "meta.info") {
            mTar->receiveContent();
        } else if (mCurrentFile == "uImage") {
            mount(QString(), "/mnt/boot", "remount,rw");
            mTar->extractContent("/mnt/boot");
        } else if (mCurrentFile == "rootfs.tar") {
            qDebug() << "Formatting rootfs";
            execute("mkfs." + mMetaInfo["filesystemType"], QStringList() << mMetaInfo["rootDevice"]);
            mount(mMetaInfo["rootDevice"], "/mnt/root");
            mTar->extractContent("/mnt/root");
        } else if (mCurrentFile == "keys.tar") {
            mount(QString(), "/mnt/boot", "remount,rw");
            execute("rm", QStringList() << "-r" << "/mnt/boot/update/keys/");
            execute("mkdir", QStringList() << "/mnt/boot/update/keys/");
            mTar->extractContent("/mnt/boot/update/keys");
        } else {
            qFatal("Unexpected file in update data");
        }
    }
}

void Update::tarEndingFile(const QString &name)
{
    if (name == "meta.info") {
        QMap<QString,QString> map = parseMetaInfo(mTar->currentContent());

        // Some rough sanity checks
        if (!map.contains("version"))
            qFatal("No version information found" );
        if (map["version"] != "1")
            qFatal("Invalid update version");
        if (!map.contains("platform") || map["platform"].isEmpty())
            qFatal("Platform information missing");
        if (map["platform"] != readAll("/mnt/boot/update/platform").trimmed())
            qFatal("Invalid platform information");
        else
            qDebug() << "Platform matches";
        if (!map.contains("rootDevice") || map["rootDevice"].isEmpty())
            qFatal("Root device information missing");
        if (!QFile::exists(map["rootDevice"]))
            qFatal("Root device not found");
        if (!map.contains("key") || map["key"].isEmpty() || map["key"].contains('/'))
            qFatal("Invalid or no key fingerprint");
        if (!map.contains("digest") || map["digest"].isEmpty())
            qFatal("Invalid digest type");
        if (!map.contains("filesystemType") || map["filesystemType"].isEmpty() || map["filesystemType"].contains(' ') || map["filesystemType"].contains('/'))
            qFatal("Invalid filesystem type");

        mMetaInfo = map;

        if (!mTar->setVerificationData("/mnt/boot/update/keys/" + mMetaInfo["key"], mMetaInfo["digest"]))
            qFatal("Loading of certificate failed");

        // Now verify the meta.info itself
        if (mTar->verifyCurrentContent(mCurrentSignature)) {
            qDebug() << "meta.info VERIFIED OK";
        } else {
            qDebug() << "meta.info VERIFICATION FAILED";
            qFatal("VERIFICATION FAILED");
            return;
        }

    } else if (name.endsWith(".sig")) {
        mCurrentSignature = mTar->currentContent();
        qDebug() << "Received signature with" << mCurrentSignature.size() << "bytes.";
    } else {
        if (mTar->checkSignature(mCurrentSignature)) {
            qDebug() << "VERIFIED OK";
            // FIXME: do something here
        } else {
            qDebug() << "VERIFICATION FAILED";
            qFatal("VERIFICATION FAILED");
        }
    }

    mTar->continueContent();
}

void Update::tarFinished()
{
    qDebug() << "Update finished";
    mount(QString(), "/mnt/boot", "remount,rw");
    if (!writeAll("/mnt/boot/update/state", "t"))
        qFatal("Could not set state to testing");
    mount(QString(), "/mnt/boot", "remount,ro");
    qApp->exit();
}
