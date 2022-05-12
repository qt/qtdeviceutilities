/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
******************************************************************************/
#include "qnetworksettingsservice.h"
#include "qnetworksettingsservice_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QNetworkSettingsService
    \inmodule QtNetworkSettings

    \brief The QNetworkSettingsService class represents a network service.

    \sa QNetworkSettingsManager::services()
*/

/*!
    \property QNetworkSettingsService::id
    \readonly
    \brief Holds a unique ID of this service.
*/

/*!
    \property QNetworkSettingsService::name
    \readonly
    \brief Holds the name of this service.
*/


/*!
    \property QNetworkSettingsService::state
    \readonly
    \brief Holds the state of this service.

    \sa QNetworkSettingsState::State
*/

/*!
    \property QNetworkSettingsService::type
    \readonly
    \brief Holds the type of this service.

    \sa QNetworkSettingsType::Type
*/

/*!
    \property QNetworkSettingsService::ipv4
    \readonly
    \brief Holds the IPv4 address for this service.
*/

/*!
    \property QNetworkSettingsService::ipv6
    \readonly
    \brief Holds the IPv6 address for this service.
*/

/*!
    \property QNetworkSettingsService::proxy
    \readonly
    \brief Holds the proxy settings for this service.
*/

/*!
    \property QNetworkSettingsService::wirelessConfig
    \readonly
    \brief Holds the wireless configuration for this service.
*/

/*!
    \property QNetworkSettingsService::domains
    \readonly
    \brief The model containing the domains associated with this service.

    This property can be used as a model for a view that lists the domain
    addresses associated with this service.
*/

/*!
    \property QNetworkSettingsService::nameservers
    \readonly
    \brief The model containing the domain name servers associated with this
           service.

    This property can be used as a model for a view that lists the domain name
    server (DNS) addresses associated with this service.
*/

/*!
    \property QNetworkSettingsService::autoConnect
    \brief Holds autoconnect property for this service.

    Autoconnect property determines whether the service connects automatically
    when it is available.
*/

/*!
    \fn QNetworkSettingsService::connectionStateCleared()

    This signal is emitted when the connection state is cleared.
*/

/*!
    \fn QNetworkSettingsService::serviceConnected(QNetworkSettingsService* service);

    This signal is emitted when the connection to the network \a service is
    created.
*/

/*!
    \fn QNetworkSettingsService::serviceDisconnected(QNetworkSettingsService* service);

    This signal is emitted when the connection to the network \a service is
    cut.
*/

/*!
    \qmltype NetworkService
    \inqmlmodule QtDeviceUtilities.NetworkSettings
    \brief Represents a network service.

    The NetworkService QML type represents a network service.

    Instances of NetworkService cannot be created directly; instead, they can
    be retrieved via NetworkSettingsManager.

    \sa {NetworkSettingsManager::services}{NetworkSettingsManager.services}
*/

/*!
    Creates a network service with the identifier \a aServiceId and
    parent \a parent.
*/
QNetworkSettingsService::QNetworkSettingsService(const QString& aServiceId, QObject* parent) :
   QObject(parent)
  ,d_ptr(new QNetworkSettingsServicePrivate(aServiceId, this))
{

}

/*!
    \qmlproperty string NetworkService::id
    \readonly
    \brief Holds a unique ID of this service.
*/

/*!
    Returns the unique identifier of the network service.
*/
QString QNetworkSettingsService::id() const
{
    Q_D(const QNetworkSettingsService);
    return d->m_id;
}

/*!
    \qmlproperty string NetworkService::name
    \readonly
    \brief Holds the name of this service.
*/

/*!
    Returns the network service name.
*/
QString QNetworkSettingsService::name() const
{
    Q_D(const QNetworkSettingsService);
    return d->m_name;
}

/*!
    \fn void QNetworkSettingsService::setAutoConnect(bool autoconnect)

    Sets automatic connections to the network service to \a autoconnect.
*/
void QNetworkSettingsService::setAutoConnect(const bool autoconnect)
{
    Q_D(QNetworkSettingsService);
    d->setAutoConnect(autoconnect);
}

/*!
    Returns the automatic connection status of the network service.
*/
bool QNetworkSettingsService::autoConnect() const
{
    Q_D(const QNetworkSettingsService);
    return d->autoConnect();
}

/*!
    \qmlproperty enumeration NetworkService::state
    \readonly
    \brief Holds the state of this service.

    See \l [QML] {NetworkInterface::state}{NetworkInterface.state}
    for possible states.
*/

/*!
    Returns the network service state.

    \sa QNetworkSettingsState::State
*/
QNetworkSettingsState::State QNetworkSettingsService::state()
{
    Q_D(QNetworkSettingsService);
    return d->m_state.state();
}

/*!
    \qmlproperty enumeration NetworkService::type
    \readonly
    \brief Holds the type of this service.

    See \l [QML] {NetworkInterface::type}{NetworkInterface.type}
    for possible types.
*/

/*!
    Returns the network service type.

    \sa QNetworkSettingsType::Type
*/
QNetworkSettingsType::Type QNetworkSettingsService::type()
{
    Q_D(QNetworkSettingsService);
    return d->m_type.type();
}

/*!
    \qmlproperty NetworkSettingsIPv4 NetworkService::ipv4
    \readonly
    \brief Holds the IPv4 address for this service.
*/

/*!
    Returns the IPv4 address of the network service.
*/
QNetworkSettingsIPv4* QNetworkSettingsService::ipv4()
{
    Q_D(QNetworkSettingsService);
    return &d->m_ipv4config;
}

/*!
    \qmlproperty NetworkSettingsIPv6 NetworkService::ipv6
    \readonly
    \brief Holds the IPv6 address for this service.
*/

/*!
    Returns the IPv6 address of the network service.
*/
QNetworkSettingsIPv6* QNetworkSettingsService::ipv6()
{
    Q_D(QNetworkSettingsService);
    return &d->m_ipv6config;
}

/*!
    \qmlproperty NetworkSettingsProxy NetworkService::proxy
    \readonly
    \brief Holds the proxy settings for this service.
*/

/*!
    Returns the address of proxy used for the network service.
*/
QNetworkSettingsProxy* QNetworkSettingsService::proxy()
{
    Q_D(QNetworkSettingsService);
    return &d->m_proxyConfig;
}

/*!
    \qmlproperty NetworkSettingsWireless NetworkService::wirelessConfig
    \readonly
    \brief Holds the wireless configuration for this service.
*/

/*!
    Returns the wireless configuration of the network service.
*/
QNetworkSettingsWireless* QNetworkSettingsService::wirelessConfig()
{
    Q_D(QNetworkSettingsService);
    return &d->m_wifiConfig;
}

/*!
    Sets the placeholder state of the network service to
    \a placeholderState.
*/
void QNetworkSettingsService::setPlaceholderState(bool placeholderState)
{
    Q_D(QNetworkSettingsService);
    d->setPlaceholderState(placeholderState);
}

/*!
    Returns the placeholder state of the network service.
*/
bool QNetworkSettingsService::placeholderState() const
{
    Q_D(const QNetworkSettingsService);
    return d->placeholderState();
}

/*!
    \qmlproperty object NetworkService::domains
    \readonly
    \brief The model containing the domains associated with this service.

    The \e domains property can be used as a model for a view
    that lists the domain addresses associated with this service.

    \sa domains.count, domains.append(), domains.remove(), domains.resetChanges()
*/

/*!
    \qmlproperty int NetworkService::domains.count
    \readonly
    \brief Holds the number of domain addresses in the \l domains model.
*/

/*!
   \qmlmethod void NetworkService::domains.append(string address)
   \brief Adds \a address into the \l domains model.
*/

/*!
   \qmlmethod void NetworkService::domains.remove(int index)
   \brief Removes the entry at index \a index from the \l domains model.
*/

/*!
   \qmlmethod void NetworkService::domains.resetChanges()
   \brief Clears unsaved changes from the \l domains model.
*/

/*!
    Returns the model containing the domains associated with this network
    settings service.
*/
QAbstractItemModel* QNetworkSettingsService::domains()
{
    Q_D(QNetworkSettingsService);
    return &d->m_domainsConfig;
}

/*!
    \qmlproperty object NetworkService::nameservers
    \readonly
    \brief The model containing the domain name servers associated with this
           service.

    The \e nameservers property can be used as a model for a view
    that lists the domain name server (DNS) addresses associated with this
    service.

    \sa nameservers.count, nameservers.append(), nameservers.remove(), nameservers.resetChanges()
*/

/*!
    \qmlproperty int NetworkService::nameservers.count
    \readonly
    \brief Holds the number of domain name server addresses in the
           \l nameservers model.
*/

/*!
   \qmlmethod void NetworkService::nameservers.append(string address)
   \brief Adds \a address into the \l nameservers model.
*/

/*!
   \qmlmethod void NetworkService::nameservers.remove(int index)
   \brief Removes the entry at index \a index from the \l nameservers model.
*/

/*!
   \qmlmethod void NetworkService::nameservers.resetChanges()
   \brief Clears unsaved changes from the \l nameservers model.
*/

/*!
    Returns the model containing the domain name servers associated with this
    network service.
*/
QAbstractItemModel* QNetworkSettingsService::nameservers()
{
    Q_D(QNetworkSettingsService);
    return &d->m_nameserverConfig;
}

/*!
    \qmlmethod void NetworkService::setupIpv4Config()
    \brief Sets up the IPv4 configuration.

    Call this method after changing the IPv4 settings.

    \sa ipv4
*/

/*!
    Sets up the IPv4 configuration.

    Call this method after changing the IPv4 settings.

    \sa ipv4()
*/
void QNetworkSettingsService::setupIpv4Config()
{
    Q_D(QNetworkSettingsService);
    d->setupIpv4Config();
}

/*!
    \qmlmethod void NetworkService::setupIpv6Config()
    \brief Sets up the IPv6 configuration.

    Call this method after changing the IPv6 settings.

    \sa ipv6
*/

/*!
    Sets up the IPv6 configuration.

    Call this method after changing the IPv6 settings.

    \sa ipv6()
*/
void QNetworkSettingsService::setupIpv6Config()
{
    Q_D(QNetworkSettingsService);
    d->setupIpv6Config();
}

/*!
    \qmlmethod void NetworkService::setupNameserversConfig()
    \brief Sets up the domain name server configuration.

    Call this method after modifying the list of domain name servers.

    \sa nameservers
*/

/*!
    Sets up the domain name server configuration.

    Call this method after changing the domain name server settings.

    \sa nameservers
*/
void QNetworkSettingsService::setupNameserversConfig()
{
    Q_D(QNetworkSettingsService);
    d->setupNameserversConfig();
}

/*!
    \qmlmethod void NetworkService::setupDomainsConfig()
    \brief Sets up the domains configuration.

    Call this method after modifying the list of domain addresses.

    \sa domains
*/

/*!
    Sets up the domain configuration.

    Call this method after modifying the list of domain addresses.

    \sa domains
*/
void QNetworkSettingsService::setupDomainsConfig()
{
    Q_D(QNetworkSettingsService);
    d->setupDomainsConfig();
}

/*!
    \qmlmethod void NetworkService::setupNetworkSettingsProxy()
    \brief Sets up the network proxy configuration.

    Call this method after modifying the network proxy settings.

    \sa proxy
*/

/*!
    Sets up the network proxy configuration.

    Call this method after modifying the network proxy settings.

    \sa proxy
*/
void QNetworkSettingsService::setupNetworkSettingsProxy()
{
    Q_D(QNetworkSettingsService);
    d->setupQNetworkSettingsProxy();
}

/*!
    \qmlmethod void NetworkService::connectService()
    \brief Initiates the process of connecting to this network service.

    \sa disconnectService()
*/

/*!
    Initiates the process of connecting to this network service.

    \sa disconnectService()
*/
void QNetworkSettingsService::connectService()
{
    emit connectionStateCleared();
    doConnectService();
}

/*!
    Creates a connection to this network service.

    \sa connectService(), disconnectService()
*/
void QNetworkSettingsService::doConnectService()
{
    Q_D(QNetworkSettingsService);
    d->connectService();
}

/*!
    \qmlmethod void NetworkService::disconnectService()
    \brief Disconnects this service.

    \sa connectService()
*/

/*!
    Disconnects this service.

    \sa connectService()
*/
void QNetworkSettingsService::disconnectService()
{
    Q_D(QNetworkSettingsService);
    d->disconnectService();
}

/*!
    \qmlmethod void NetworkService::removeService()
    \brief Removes this service from the service cache and clears
    any remembered credentials.
*/

/*!
    Removes this service from the service cache and clears any remembered
    credentials.
*/
void QNetworkSettingsService::removeService()
{
    Q_D(QNetworkSettingsService);
    d->removeService();
}

QT_END_NAMESPACE
