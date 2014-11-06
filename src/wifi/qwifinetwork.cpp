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
#include "qwifinetwork_p.h"

QT_BEGIN_NAMESPACE

/*!
    \qmltype WifiNetwork
    \inqmlmodule QtWifi
    \ingroup wifi-qmltypes
    \brief Represents a single WiFi network access point.

    WifiNetwork provides a various information about WiFi network, like network
    name, siganl strength and supported security protocols. Instances of WifiNetwork cannot
    be created directly from the QML system, see WifiManager::networks.
*/

/*!
    \qmlproperty string WifiNetwork::bssid
    \readonly

    This property holds basic service set identification of a network, used to uniquely
    identify BSS.

*/

/*!
    \qmlproperty string WifiNetwork::ssid
    \readonly

    This property holds a network name. The SSID is the informal (human) name of BSS.
*/

/*!
    \qmlproperty int WifiNetwork::signalStrength
    \readonly

    This property holds the current strength of a WiFi signal, measured in dBm. New readings are
    taken every 5 seconds.

    \sa signalStrengthChanged
*/

/*!
    \qmlproperty bool WifiNetwork::supportsWPA
    \readonly

    This property holds whether network access point supports WPA security protocol.
*/

/*!
    \qmlproperty bool WifiNetwork::supportsWPA2
    \readonly

    This property holds whether network access point supports WPA2 security protocol.
*/

/*!
    \qmlproperty bool WifiNetwork::supportsWEP
    \readonly

    This property holds whether network access point supports WEP security protocol.
*/

/*!
    \qmlproperty bool WifiNetwork::supportsWPS
    \readonly

    This property holds whether network access point supports WPS security protocol.
*/

/*!
    \qmlsignal void WifiNetwork::signalStrengthChanged(int strength)

    This signal is emitted whenever signal strength has changed comparing the the
    previous reading, the new signal's strength is \a strength.

*/

QWifiNetwork::QWifiNetwork(QObject *parent)
    : QObject(parent)
    , m_isOutOfRange(false)
{
}

QWifiNetwork::~QWifiNetwork()
{
}

void QWifiNetwork::setSsid(const QString &ssid)
{
    m_ssid = ssid;
}

void QWifiNetwork::setSignalStrength(int strength)
{
    m_signalStrength = strength;
}

void QWifiNetwork::setOutOfRange(bool outOfRange)
{
    m_isOutOfRange = outOfRange;
}

QT_END_NAMESPACE
