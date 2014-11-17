/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://www.qt.io
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://www.qt.io
**
****************************************************************************/
#ifndef QWIFIINTERFACE_H
#define QWIFIINTERFACE_H

#include <QtCore/QDir>
#include <QtCore/QDebug>
#ifdef Q_OS_ANDROID
#include <hardware_legacy/wifi.h>
#include <cutils/properties.h>
#endif

class QWifiInterface : public QObject
{
    Q_OBJECT
public:
    explicit QWifiInterface(QObject *parent = 0)
        : QObject(parent) {}
    ~QWifiInterface() {}

    Q_INVOKABLE bool wifiSupported() const;
};

#endif // QWIFIHELPERS_H
