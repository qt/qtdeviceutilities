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
    \qmltype WifiDevice
    \inqmlmodule QtWifi
    \ingroup wifi-qmltypes
    \brief Represents a physical device

    Use this element to query if a device is WiFi capable before attempting
    to use functionality of WifiManager.

    \qml
    import QtWifi 1.0

    GroupBox {
        id: wifiOptions
        title: "Wifi"
        visible: false

        Component.onCompleted: {
            if (WifiDevice.wifiSupported()) {
                var component = Qt.createComponent("WifiGroupBox.qml")
                var wifi = component.createObject(wifiOptions.contentItem)
                if (wifi)
                    wifiOptions.visible = true
            } else {
                print("WiFi functionality not available on this device.")
            }
        }
    }
    \endqml
*/

/*!
    \qmlmethod bool QWifiDevice::wifiSupported()

    Returns true if the device is WiFi capable - WiFi driver and firmware has been
    successfully loaded by the system, otherwise returns false.

    \sa wifiInterfaceName
*/

QWifiDevice::QWifiDevice()
{
}

QWifiDevice::~QWifiDevice()
{
}

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
    \fn QByteArray QWifiDevice::wifiInterfaceName()

    Returns WiFi interface name.

    \note On Android WiFi interface name is read from "wifi.interface" system property.
    On Linux WiFi interface name is read from B2QT_WIFI_INTERFACE environmental variable if
    it is set. The default interface name is "wlan0" if reading the designated places does not
    provide an interface name.

    /sa setWifiInterfaceName
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
    \fn void QWifiDevice::setWifiInterfaceName(const QByteArray &name)

    A conveniece method for settings WiFi interface name.
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
