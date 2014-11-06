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

#include <QByteArray>
#include <QFile>
#include <QPair>
#include <QDebug>

QByteArray readAll(const QString &fileName)
{
    QByteArray rc;
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Could not open file:" << fileName;
        return rc;
    }

    rc = file.readAll();
    file.close();
    return rc;
}


static QList<QPair<QString, QString> > getMounts()
{
    QList<QPair<QString, QString> > rc;

    QByteArray ba = readAll("/proc/mounts");
    foreach (const QString &line, ba.split('\n')) {
        if (line.isEmpty())
            continue;

        QStringList token = line.split(' ');
        if (token.size() < 2) {
            qWarning() << "Error parsing mounts:" << line;
            continue;
        }
        rc.append(qMakePair(token[0], token[1]));
    }
    return rc;
}

bool checkForDeviceMounted(const QString &device, QString &mountPoint, QString &partition)
{
    QList<QPair<QString, QString> > mounts = getMounts();
    if (mounts.isEmpty()) {
        qWarning() << "No mounts found";
        return false;
    }

    foreach (const auto &item, mounts) {
        if (item.first.startsWith(device)) {
            mountPoint = item.second;
            partition = item.first;
            return true;
        }
    }
    mountPoint.clear();
    partition.clear();
    return false;
}

