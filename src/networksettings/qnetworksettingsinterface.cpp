// Copyright (C) 2019 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include "qnetworksettingsinterface.h"
#include "qnetworksettingsinterface_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QNetworkSettingsInterface
    \inmodule QtNetworkSettings
    \brief Represents a network interface.

    The QNetworkSettingsInterface class represents a network interface attached
    to the host.

    Instances of this class cannot be created directly. Instead, they can be
    retrieved via QNetworkSettingsManager::interfaces().
*/

/*!
    \property QNetworkSettingsInterface::state
    \readonly
    \brief Holds the state of the network interface.

    \sa QNetworkSettingsState::State
*/

/*!
    \property QNetworkSettingsInterface::name
    \readonly
    \brief Holds the name of the network interface.
*/

/*!
    \property QNetworkSettingsInterface::powered
    \brief Holds whether the network interface is powered on of off.
*/

/*!
    \property QNetworkSettingsInterface::type
    \readonly
    \brief Holds the type of the network interface.

    \sa QNetworkSettingsType::Type
*/

/*!
    \qmltype NetworkInterface
    \inqmlmodule QtDeviceUtilities.NetworkSettings
    \brief Represents a network interface.

    The NetworkInterface QML type represents a network interface attached
    to the host.

    Instances of NetworkInterface cannot be created directly; instead, they can
    be retrieved via NetworkSettingsManager.

    \sa {NetworkSettingsManager::interfaces}{NetworkSettingsManager.interfaces}
*/


/*!
    Creates a new network interface with the parent \a parent.
*/
QNetworkSettingsInterface::QNetworkSettingsInterface(QObject *parent) :
    QObject(parent)
    ,d_ptr(new QNetworkSettingsInterfacePrivate(this))
{

}

/*!
    \qmlproperty string NetworkInterface::name
    \readonly
    \brief Holds the name of the network interface.
*/

/*!
    Returns the name of the network interface.
*/
QString QNetworkSettingsInterface::name() const
{
    Q_D(const QNetworkSettingsInterface);
    return d->name();
}

/*!
    \qmlproperty enumeration NetworkInterface::state
    \readonly
    \brief Holds the state of the network interface.

    Possible values:

    \value NetworkSettingsState.Idle
           Idle

    \value NetworkSettingsState.Failure
           Failed to connect

    \value NetworkSettingsState.Association
           Authentication in progress

    \value NetworkSettingsState.Configuration
           Configuration in progress

    \value NetworkSettingsState.Ready
           Connected to a network

    \value NetworkSettingsState.Disconnect
           Disconnected from a network

    \value NetworkSettingsState.Online
           Online (acquired an IP address)

    \value NetworkSettingsState.Undefined
           Undefined state.
*/

/*!
    Returns the state of the network interface.
*/
QNetworkSettingsState::State QNetworkSettingsInterface::state()
{
    Q_D(QNetworkSettingsInterface);
    return d->state();
}

/*!
    \qmlproperty enumeration NetworkInterface::type
    \readonly
    \brief Holds the type of the network interface.

    Possible values:

    \value NetworkSettingsType.Wired     Wired network
    \value NetworkSettingsType.Wifi      Wifi network
    \value NetworkSettingsType.Bluetooth Bluetooth network
    \value NetworkSettingsType.Unknown   Unknown network type
*/

/*!
    Returns the type of the network interface.
*/
QNetworkSettingsType::Type QNetworkSettingsInterface::type()
{
    Q_D(QNetworkSettingsInterface);
    return d->type();
}

/*!
    \qmlproperty bool NetworkInterface::powered
    \brief Holds whether the network interface is powered on or off.
*/

/*!
    Returns whether the network interface is powered on or off.
*/
bool QNetworkSettingsInterface::powered() const
{
    Q_D(const QNetworkSettingsInterface);
    return d->powered();
}

/*!
    Sets the powered state in the network interface to \a powered.
*/
void QNetworkSettingsInterface::setPowered(const bool powered)
{
    Q_D(QNetworkSettingsInterface);
    d->setPowered(powered);
}

/*!
    \qmlmethod void NetworkInterface::scanServices()
    \brief Initiates a scan for network interface services.
*/

/*!
    Initiates a scan for network interface services.
*/
void QNetworkSettingsInterface::scanServices()
{
    Q_D(QNetworkSettingsInterface);
    d->scan();
}

QT_END_NAMESPACE
