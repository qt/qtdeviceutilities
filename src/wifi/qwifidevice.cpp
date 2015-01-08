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
#ifdef Q_OS_ANDROID_NO_SDK
#include <hardware_legacy/wifi.h>
#include <cutils/properties.h>
#endif

QT_BEGIN_NAMESPACE

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
{
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
#ifdef Q_OS_ANDROID_NO_SDK
    const char *fwpath = 0;
    // reload wifi firmware
    fwpath = (char *)wifi_get_fw_path(WIFI_GET_FW_PATH_STA);
    if (!fwpath) {
        qCWarning(B2QT_WIFI) << "failed to get firmware path";
        return false;
    }
    if (wifi_change_fw_path((const char *)fwpath)) {
        qCWarning(B2QT_WIFI) << "failed to change firmware path";
        return false;
    }
#endif
    QByteArray ifc = wifiInterfaceName();
    bool hasInterface = QDir().exists(QString::fromLatin1("/sys/class/net/" + ifc));
    if (!hasInterface)
        qCWarning(B2QT_WIFI) << "could not find wifi interface in \"/sys/class/net/\", "
                                 "looking for interface named: " << ifc;
#ifdef Q_OS_ANDROID_NO_SDK
    if (hasInterface && wifi_load_driver() == 0) {
        return true;
    } else {
        qCWarning(B2QT_WIFI) << "wifi driver is not available";
        return false;
    }
#else
    return hasInterface;
#endif
}

/*!
    Returns Wifi interface name.

    \note On Android, the Wifi interface name is read from "wifi.interface"
    system property. On Linux, it is read from the \c B2QT_WIFI_INTERFACE
    environment variable if it is set, otherwise, the default interface
    name ("\e{wlan0}") is used.

    \sa setWifiInterfaceName()
 */
QByteArray QWifiDevice::wifiInterfaceName()
{
    QByteArray ifc;
#ifdef Q_OS_ANDROID_NO_SDK
    char interface[PROPERTY_VALUE_MAX];
    property_get("wifi.interface", interface, NULL);
    ifc = interface[0] == '\0' ? "wlan0" : interface;
#else
    ifc = qEnvironmentVariableIsSet("B2QT_WIFI_INTERFACE")
            ? qgetenv("B2QT_WIFI_INTERFACE") : "wlan0";
#endif
    return ifc;
}

/*!
    A conveniece method to set the Wifi interface \a name.

    \sa wifiInterfaceName()
 */
void QWifiDevice::setWifiInterfaceName(const QByteArray &name)
{
#ifdef Q_OS_ANDROID_NO_SDK
    property_set("wifi.interface", name);
#else
    qputenv("B2QT_WIFI_INTERFACE", name);
#endif
}

QT_END_NAMESPACE
