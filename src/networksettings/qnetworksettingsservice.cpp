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
#include "qnetworksettingsservice.h"
#include "qnetworksettingsservice_p.h"

/*!
    \qmltype NetworkService
    \inqmlmodule QtDeviceUtilities.NetworkSettings
    \brief Represents a network service.

    The NetworkService QML type represents a network service.

    Instances of NetworkService cannot be created directly; instead, they can
    be retrieved via NetworkSettingsManager.

    \sa {NetworkSettingsManager::services}{NetworkSettingsManager.services}
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
QString QNetworkSettingsService::name() const
{
    Q_D(const QNetworkSettingsService);
    return d->m_name;
}

/*!
    \qmlmethod void NetworkService::setAutoConnect(bool auto)
*/
void QNetworkSettingsService::setAutoConnect(const bool autoconnect)
{
    Q_UNUSED(autoconnect);
}

/*!
    \qmlproperty enumeration NetworkService::state
    \readonly
    \brief Holds the state of this service.

    See \l [QML] {NetworkInterface::state}{NetworkInterface.state}
    for possible states.
*/
QNetworkSettingsState::States QNetworkSettingsService::state()
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
QNetworkSettingsType::Types QNetworkSettingsService::type()
{
    Q_D(QNetworkSettingsService);
    return d->m_type.type();
}

/*!
    \qmlproperty NetworkSettingsIPv4 NetworkService::ipv4
    \readonly
    \brief Holds the IPv4 address for this service.
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
QNetworkSettingsWireless* QNetworkSettingsService::wirelessConfig()
{
    Q_D(QNetworkSettingsService);
    return &d->m_wifiConfig;
}

void QNetworkSettingsService::setPlaceholderState(bool placeholderState)
{
    Q_D(QNetworkSettingsService);
    d->setPlaceholderState(placeholderState);
}

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
void QNetworkSettingsService::connectService()
{
    Q_D(QNetworkSettingsService);
    d->connectService();
}

/*!
    \qmlmethod void NetworkService::disconnectService()
    \brief Disconnects this service.

    \sa connectService()
*/
void QNetworkSettingsService::disconnectService()
{
    Q_D(QNetworkSettingsService);
    d->disconnectService();
}
