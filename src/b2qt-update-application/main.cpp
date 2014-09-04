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

#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <unistd.h>
#include <sys/reboot.h>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "update.h"
#include "filewrapper.h"

#define UPDATE_MOUNTPOINT "/mnt/update"

QStringList mounts;

void cleanup()
{
    foreach (const QString &m, mounts) {
        QProcess::execute("umount", QStringList() << m);
    }
    mounts.clear();
}

void error(const QString &message)
{
    fprintf(stderr, "%s\n", message.toLocal8Bit().constData());
    cleanup();
    exit(1);
}

bool execute(const QString &binary, const QStringList &arguments)
{
    int rc = QProcess::execute(binary, arguments);
    if (rc != 0)
        error("Failed to execute command '" + binary + " " + arguments.join(" "));
    return rc == 0;
}

bool mount(const QString &device, const QString &mountpoint, const QString &arguments = QString())
{
    QStringList tmp;
    if (!arguments.isEmpty())
        tmp << "-o" << arguments;

    if (!execute("mount", QStringList() << tmp << device << mountpoint))
        return false;

    if (!arguments.contains("remount"))
        mounts << mountpoint;
    return true;
}

QByteArray readAll(const QString &fileName)
{
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly)) {
        qWarning() << "Could not read" << fileName;
        return QByteArray();
    }

    return f.readAll();
}

bool writeAll(const QString &fileName, const QByteArray &content)
{
    QFile f(fileName);
    if (!f.open(QFile::WriteOnly)) {
        qWarning() << "Could not write" << fileName;
        return false;
    }
    if (f.write(content) != content.size()) {
        qWarning() << "write size mismatch";
        return false;
    }
    f.close();
    return true;
}

QStringList find_usb_storage()
{
    QStringList rc;
    QDir d("/sys/dev/block");

    foreach (QString s, d.entryList()) {
        QFileInfo fi(d.absoluteFilePath(s));
        QString path = fi.canonicalFilePath();
        if (path.contains("/usb"))
            rc += path.mid(path.lastIndexOf('/'));
    }
    return rc;
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    mount(QString(), "/", "remount,rw");
    execute("mkdir", QStringList() << "-p" << "/mnt/boot");
    execute("mkdir", QStringList() << "-p" << "/mnt/root");
    execute("mkdir", QStringList() << "-p" << UPDATE_MOUNTPOINT);
    mount("/dev/mmcblk0p1", "/mnt/boot", "ro");

    QByteArray update_state = readAll("/mnt/boot/update/state").trimmed();
    if (update_state.isEmpty())
        qFatal("Update state is empty");

    if (update_state == "v")
        qFatal("Update state is 'valid', this should not happen");
    else if (update_state == "t")
        qDebug() << "Update state is 'testing', this should not happen";
    else if (update_state == "u")
        qDebug() << "Update state is 'update'";
    else
        qDebug() << "Unknown update state:" << update_state;

    QStringList usbsticks = find_usb_storage();

    qDebug() << "Found USB storage devices:" << usbsticks;

    bool update_found = false;
    foreach (QString s, usbsticks) {
      if (QProcess::execute("mount", QStringList() << "-o" << "ro" << "/dev/" + s << UPDATE_MOUNTPOINT) == 0) {
          mounts << UPDATE_MOUNTPOINT;
         qDebug() << "mount successful";

         if (QFile::exists(UPDATE_MOUNTPOINT "/b2qt-update")) {
            qDebug() << "update found";
            update_found = true;
            break;
         }
         execute("umount", QStringList() << UPDATE_MOUNTPOINT);
         mounts.removeAt(mounts.lastIndexOf(UPDATE_MOUNTPOINT));
         qDebug() << "no update found";
      } else {
          qDebug() << "mount of" << s << "failed";
      }
    }

    Update update;

    if (update_found) {
          FileWrapper *fw = new FileWrapper(UPDATE_MOUNTPOINT "/b2qt-update", &update);
          if (!fw->open(QFile::ReadOnly))
              qFatal("Failed to open update");
          qDebug() << "Starting update from USB";
          update.setDevice(fw);
    } else {
          QByteArray update_source = readAll("/mnt/boot/update/source").trimmed();
          if (update_source.isEmpty()) {
              execute("umount", QStringList() << "/mnt/boot"); // FIXME
              mounts.removeAt(mounts.lastIndexOf("/mnt/boot"));
              qFatal("Update source is empty");
          }

          execute("udhcpc", QStringList() << "-i" << "eth0");

          QNetworkAccessManager *manager = new QNetworkAccessManager(0);
          QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(update_source)));
          reply->setReadBufferSize(2000000); // 2 MB

          QObject::connect(reply, (void (QNetworkReply::*)(QNetworkReply::NetworkError))&QNetworkReply::error,
                     [](QNetworkReply::NetworkError e){qDebug() << "network error" << e;});
          QObject::connect(reply, &QNetworkReply::sslErrors,
                      [](QList<QSslError>){ qDebug() << "ssl errors";});

          qDebug() << "Starting update from Internet" << update_source;
          update.setDevice(reply);
    }

    app.exec();

    qDebug() << "unmount";
    cleanup();

    qDebug() << "sync";
    sync();
    qDebug() << "reboot; waiting 2 seconds";
    sleep(2);
    reboot(RB_AUTOBOOT);
    return 0;
}
