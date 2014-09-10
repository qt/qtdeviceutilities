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
#include <sys/reboot.h>
#include <QFile>
#include <QDebug>
#include <unistd.h>
#include <QStringList>
#include <QProcess>

bool mount_boot()
{
    // In some cases the boot partition is already mounted somewhere else.
    // Mounting it again is no problem but data loss will happen.
    QProcess::execute("umount", QStringList() << "/dev/mmcblk0p1"); // Ignore return value
    return QProcess::execute("mount", QStringList() << "/dev/mmcblk0p1" << "/boot") == 0;
}

bool umount_boot()
{
    return QProcess::execute("umount", QStringList() << "/boot") == 0;
}

bool finish_update()
{
    if (!mount_boot()) {
        qWarning() << "Could not mount /boot";
        return false;
    }
    QFile f("/boot/update/state");
    if (!f.open(QFile::WriteOnly)) {
        qWarning() << "Could not open file for writing";
        return false;
    }
    if (f.write("v") != 1) {
        qWarning() << "Write error";
        return false;
    }
    fsync(f.handle());
    f.close();
    if (!umount_boot()) {
        qWarning() << "Could not unmount /boot";
        return false;
    }
    return true;
}

int start_update(const QString &source)
{
    if (!mount_boot()) {
        qWarning() << "Could not mount /boot";
        return false;
    }
    {
        QFile f("/boot/update/source");
        if (!f.open(QFile::WriteOnly)) {
            qWarning() << "Could not open file for writing";
            return false;
        }
        QByteArray ba = source.toUtf8();

        if (f.write(ba) != ba.size()) {
            qWarning() << "Write error";
            return false;
        }
        fsync(f.handle());
        f.close();
    }

    {
        QFile f("/boot/update/state");
        if (!f.open(QFile::WriteOnly)) {
            qWarning() << "Could not open file for writing";
            return false;
        }
        if (f.write("u") != 1) {
            qWarning() << "Write error";
            return false;
        }
        fsync(f.handle());
        f.close();
    }
    if (!umount_boot()) {
        qWarning() << "Could not unmount /boot";
        return false;
    }
    reboot(RB_AUTOBOOT);
    return true;
}

void usage()
{
  fprintf(stderr,
    "b2qt-update [start|finish] [...]\n"
    "   start: For internet update provide a http URL as parameter\n"
    "   finish: An update\n"
    );
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QStringList args = app.arguments();

    args.removeFirst();
    if (args.size() == 0) {
        usage();
        return 1;
    }

    QString arg = args.takeFirst();

    if (arg == "finish")
        return finish_update();
    else if (arg == "start") {
        if (args.size() == 0)
            return start_update(QString());
        else
            return start_update(args.takeFirst());
    } else {
        usage();
        return 1;
    }
}
