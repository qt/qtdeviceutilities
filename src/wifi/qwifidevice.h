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
#ifndef QWIFIDEVICE_H
#define QWIFIDEVICE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QLoggingCategory>

class QWifiDevicePrivate;

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(B2QT_WIFI)

class Q_DECL_EXPORT QWifiDevice : public QObject
{
    Q_OBJECT
public:
    explicit QWifiDevice();
    virtual ~QWifiDevice();

    Q_INVOKABLE static bool wifiSupported();
    static QByteArray wifiInterfaceName();
    static void setWifiInterfaceName(const QByteArray &name);

private:
    Q_DISABLE_COPY(QWifiDevice)
    Q_DECLARE_PRIVATE(QWifiDevice)
    QWifiDevicePrivate *const d_ptr;
};

QT_END_NAMESPACE

#endif // QWIFIDEVICE_H
