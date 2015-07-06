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
#include "qwifidevice.h"

#include <QtCore/QByteArray>
#include <QtCore/QDir>

QT_BEGIN_NAMESPACE

class QWifiDevicePrivate
{
    Q_DECLARE_PUBLIC(QWifiDevice)
public:
    QWifiDevicePrivate(QWifiDevice *device);

    // methods
    void createSupplicantConfig();
    // member variables
    QWifiDevice *const q_ptr;
};

QWifiDevicePrivate::QWifiDevicePrivate(QWifiDevice *device)
    : q_ptr(device)
{
}

void QWifiDevicePrivate::createSupplicantConfig()
{
    QFile supplicantConfig(QStringLiteral("/etc/wpa_supplicant.qtwifi.conf"));
    if (supplicantConfig.exists())
        return;

    if (supplicantConfig.open(QIODevice::WriteOnly)) {
        supplicantConfig.write("ctrl_interface=/var/run/wpa_supplicant\n"
                               "ctrl_interface_group=0\n"
                               "update_config=1\n");
    } else {
        qCWarning(B2QT_WIFI) << "failed to create supplicant configuration file.";
    }
}


/*!
    \class QWifiDevice
    \inmodule B2Qt.Wifi.Cpp
    \ingroup wifi-cppclasses
    \brief Represents a physical device.

    Use this class to query if a device is Wifi capable, before attempting
    to use the functionality of QWifiManager.

    \code
    QWifiManager *m_wifiManager = 0;
    if (QWifiDevice::wifiSupported())
        m_wifiManager = QWifiManager::instance();

    if (m_wifiManager) {
        m_wifiManager->start();
        // and other wifi related code
    }
    \endcode
 */

QWifiDevice::QWifiDevice()
    : d_ptr(new QWifiDevicePrivate(this))
{
    d_ptr->createSupplicantConfig();
}

QWifiDevice::~QWifiDevice()
{
}

/*!
    Returns \c true if a device is Wifi capable - Wifi driver and firmware has been
    successfully loaded by the system, otherwise returns \c false.
*/
bool QWifiDevice::wifiSupported()
{
    QByteArray ifc = wifiInterfaceName();
    bool hasInterface = QDir().exists(QString::fromLatin1("/sys/class/net/" + ifc));
    if (!hasInterface)
        qCWarning(B2QT_WIFI) << "could not find wifi interface in \"/sys/class/net/\", "
                                 "looking for interface named: " << ifc;
    return hasInterface;
}

/*!
    Returns Wifi interface name.

    Interface name is obtained from the \c B2QT_WIFI_INTERFACE
    environment variable if it is set, otherwise, the default interface
    name ("\e{wlan0}") is used.

    \sa setWifiInterfaceName()
 */
QByteArray QWifiDevice::wifiInterfaceName()
{
    return qEnvironmentVariableIsSet("B2QT_WIFI_INTERFACE") ? qgetenv("B2QT_WIFI_INTERFACE") : "wlan0";
}

/*!
    A conveniece method to set the Wifi interface \a name.

    \sa wifiInterfaceName()
 */
void QWifiDevice::setWifiInterfaceName(const QByteArray &name)
{
    qputenv("B2QT_WIFI_INTERFACE", name);
}

QT_END_NAMESPACE
