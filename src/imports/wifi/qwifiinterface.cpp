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
#include "qwifiinterface.h"

/*!
    \qmltype Interface
    \inqmlmodule Qt.labs.wifi
    \ingroup wifi-qmltypes
    \brief The Interface element provides the module API.

    This element cannot be directly created. It can only be accessed via a namespace import.

    \code
    import Qt.labs.wifi 0.1
    import Qt.labs.wifi 0.1 as Wifi

    Component.onCompleted: {
        if (Wifi.Interface.wifiSupported()) {
            var component = Qt.createComponent("WifiMenu.qml")
        } else {
            print("WiFi functionality not available on this device.")
        }
    }
    \endcode
*/

/*!
    \qmlmethod bool Interface::wifiSupported()

    Returns true if the device is WiFi capable (provides a WiFi driver), otherwise returns false.
*/

bool QWifiInterface::wifiSupported() const
{
#ifdef Q_OS_ANDROID
    const char *fwpath = 0;
    // reload wifi firmware
    fwpath = (char *)wifi_get_fw_path(WIFI_GET_FW_PATH_STA);
    if (!fwpath) {
        qWarning() << "QWifiInterface: failed to get firmware path";
        return false;
    }
    if (wifi_change_fw_path((const char *)fwpath)) {
        qWarning() << "QWifiInterface: failed to change firmware path";
        return false;
    }
#endif
    const bool hasInterface = QDir().exists(QStringLiteral("/sys/class/net/wlan0"));
    if (!hasInterface)
        qWarning() << "QWifiInterface: could not find wifi interface in /sys/class/net/";
#ifdef Q_OS_ANDROID
    if (hasInterface && wifi_load_driver() == 0 && wifi_start_supplicant(0) == 0) {
        return true;
    } else {
        qWarning() << "QWifiInterface: wifi driver is not available";
        return false;
    }
#else
    return hasInterface;
#endif
}
