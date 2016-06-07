/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "qwifidevice.h"
#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QDir>

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
    QByteArray ifc = wifiInterfaceName();
    bool hasInterface = QDir().exists(QString::fromLatin1("/sys/class/net/" + ifc));
    if (!hasInterface)
        qCWarning(B2QT_WIFI) << "could not find wifi interface in \"/sys/class/net/\", "
                                 "looking for interface named: " << ifc;
    return hasInterface;
}

/*!
    Returns Wifi interface name.

    Interface name is read from the \c B2QT_WIFI_INTERFACE
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
