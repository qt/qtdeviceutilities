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

#include "filewrapper.h"
#include <QDebug>
#include <QTimer>

FileWrapper::FileWrapper(const QString &fileName, QObject *parent)
    : QFile(fileName, parent)
    , mTimer(new QTimer(this))
{
    mTimer->setInterval(50);
    connect(mTimer, &QTimer::timeout, this, &FileWrapper::emitReadyRead);
}

FileWrapper::~FileWrapper()
{
}

bool FileWrapper::open(OpenMode mode)
{
    bool rc = QFile::open(mode);
    if (rc) {
        mTimer->start();
    }
    return rc;
}

void FileWrapper::emitReadyRead()
{
    if (!atEnd()) {
        emit readyRead();
    } else {
      mTimer->stop();
      close();
    }
}
