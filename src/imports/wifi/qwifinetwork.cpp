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
#include "qwifinetwork.h"

/*!
    \qmltype WifiNetwork
    \inqmlmodule Qt.labs.wifi
    \brief WifiNetwork represents a single wifi network access point.

    Instances of WifiNetwork cannot be created directly from the QML system, use
    WifiManager::networks.
*/

/*!
    \qmlproperty string WifiNetwork::bssid

    This property holds basic service set identification of a network, used to uniquely
    identify BSS.

*/

/*!
    \qmlproperty string WifiNetwork::ssid

    This property holds a network name. The SSID is the informal (human) name of BSS.
*/

/*!
    \qmlproperty int WifiNetwork::signalStrength

    This property holds the current strength of a WiFi signal. New readings are
    taken every 5 seconds.

    \sa signalStrengthChanged
*/

/*!
    \qmlproperty bool WifiNetwork::supportsWPA

    This property holds whether network access point supports WPA security protocol.
*/

/*!
    \qmlproperty bool WifiNetwork::supportsWPA2

    This property holds whether network access point supports WPA2 security protocol.
*/

/*!
    \qmlproperty bool WifiNetwork::supportsWEP

    This property holds whether network access point supports WEP security protocol.
*/

/*!
    \qmlproperty bool WifiNetwork::supportsWPS

    This property holds whether network access point supports WPS security protocol.
*/

/*!
    \qmlsignal void WifiNetwork::signalStrengthChanged(int strength)

    This signal is emitted whenever signal strength has changed comparing the the
    previous reading, the new signal's strength is \a strength.

    \code
    function handleStrengthChanged(currentStrength) {
        if (currentStrength < UNTOLERABLE_SPEED)
            print("network speed has dropped under " + UNTOLERABLE_SPEED)
    }

    Component.onCompleted: {
        network.onSignalStrengthChanged.connect(handleStrengthChanged)
    }
    \endcode
*/

QWifiNetwork::QWifiNetwork()
{
}

void QWifiNetwork::setSignalStrength(int strength)
{
    if (m_signalStrength == strength)
        return;
    m_signalStrength = strength;
    emit signalStrengthChanged(m_signalStrength);
}
