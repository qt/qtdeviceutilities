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

#ifndef TAR_H
#define TAR_H

#include <QObject>
#include <openssl/x509.h>
class QIODevice;
class QProcess;
#define USTAR_HEADER_SIZE 512

struct ustar_header
{
char name[100];             /* File name.  Null-terminated if room. */
char mode[8];               /* Permissions as octal string. */
char uid[8];                /* User ID as octal string. */
char gid[8];                /* Group ID as octal string. */
char size[12];              /* File size in bytes as octal string. */
char mtime[12];             /* Modification time in seconds from Jan 1, 1970, as octal string. */
char chksum[8];             /* Sum of octets in header as octal string. */
char typeflag;              /* An enum ustar_type value. */
char linkname[100];         /* Name of link target.  Null-terminated if room. */
char magic[6];              /* "ustar\0" */
char version[2];            /* "00" */
char uname[32];             /* User name, always null-terminated. */
char gname[32];             /* Group name, always null-terminated. */
char devmajor[8];           /* Device major number as octal string. */
char devminor[8];           /* Device minor number as octal string. */
char prefix[155];           /* Prefix to file name.  Null-terminated if room. */
char padding[12];           /* Pad to 512 bytes. */
} __attribute__((packed));

class Tar : public QObject
{
    Q_OBJECT

public:
    Tar(QIODevice *source);
    virtual ~Tar();

    const QByteArray &currentContent() const;
    unsigned long currentSize() const;
    QString currentFileName() const;

    enum State { WaitForHeader, WaitForDecision, Extract, Receive, Skip, WaitForContinue};
    bool setVerificationData(const QString &certificateFileName, const QString &md);
    bool checkSignature(const QByteArray &signature);
    bool verifyCurrentContent(const QByteArray &signature);

signals:
    void startingFile(const QString &name);
    void endingFile(const QString &name);
    void finished();

public slots:
    void extractContent(const QString &targetDir);
    void receiveContent();
    void skipContent();
    void dataIncoming();
    void continueContent();

private slots:
    void aboutToClose();

private:
    void setupMDContext();
    void updateMDContext(QByteArray);

    QIODevice *mSource;
    qint64 mSize;
    qint64 mRemainingSize; // File Size + Padding
    qint64 mRemainingFileBytes; // File Size without Padding
    ustar_header mHeader;
    QByteArray mContent;
    QProcess *mProcess;
    State mState;
    X509 *x509;
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
};

#endif // TAR_H
