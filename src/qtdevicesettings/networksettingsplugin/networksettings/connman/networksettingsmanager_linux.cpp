/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QQmlPropertyMap>

#include "networkconfig.h"
#include "networksettingsmanager_linux_p.h"
#include "connman_manager_interface.cpp"
#include "moc_connman_manager_interface.cpp"
#include "connman_service_interface.h"
#include "connman_technology_interface.h"

const QString PropertyIPv4("IPv4");
const QString PropertyIPv4Config("IPv4.Configuration");
const QString PropertyIPv6("IPv6");
const QString PropertyIPv6Config("IPv6.Configuration");
const QString PropertyNameservers("Nameservers");
const QString PropertyNameserversConfig("Nameservers.Configuration");
const QString PropertyDomains("Domains");
const QString PropertyDomainsConfig("Domains.Configuration");
const QString PropertyProxy("Proxy");
const QString PropertyProxyConfig("Proxy.Configuration");
const QString PropertyState("State");
const QString PropertyName("Name");
const QString PropertyType("Type");
const QString PropertyConnected("Connected");
const QString PropertyPowered("Powered");
const QString PropertyAddress("Address");
const QString PropertyNetMask("Netmask");
const QString PropertyGateway("Gateway");
const QString PropertyPrefixLength("PrefixLength");
const QString PropertyMethod("Method");
const QString PropertyPrivacy("Privacy");
const QString PropertyUrl("Url");
const QString PropertyServers("Servers");
const QString PropertyExcludes("Excludes");
const QString AttributeAuto("auto");
const QString AttributeDhcp("dhcp");
const QString AttributeManual("manual");
const QString AttributeOff("off");
const QString AttributeDisabled("disabled");
const QString AttributeEnabled("enabled");
const QString AttributePreferred("preferred");
const QString AttributeDirect("direct");
const QString AttributeWifi("wifi");
const QString AttributeEthernet("ethernet");
const QString AttributeIdle("idle");
const QString AttributeFailure("failure");
const QString AttributeAssociation("association");
const QString AttributeConfiguration("configuration");
const QString AttributeReady("ready");
const QString AttributeDisconnect("disconnect");
const QString AttributeOneline("online");

//Conversion operations for datatypes
const QDBusArgument &operator>>(const QDBusArgument &argument, ConnmanMap &obj)
{
    argument.beginStructure();
    argument >> obj.objectPath >> obj.propertyMap;
    argument.endStructure();
    return argument;
}

QDBusArgument &operator<<(QDBusArgument &argument, const ConnmanMap &obj)
{
    argument.beginStructure();
    argument << obj.objectPath << obj.propertyMap;
    argument.endStructure();
    return argument;
}

const QVariantMap &operator>>(const QVariantMap &argument, IPv4Config &obj)
{
    obj.setAddress(argument[PropertyAddress].toString());
    obj.setMask(argument[PropertyNetMask].toString());
    obj.setGateway(argument[PropertyGateway].toString());

    QString method = argument[PropertyMethod].toString();

    if (method == AttributeDhcp) {
        obj.setMethod(IPv4Config::Dhcp);
    }
    else if (method == AttributeManual) {
        obj.setMethod(IPv4Config::Manual);
    }
    else if (method == AttributeOff) {
        obj.setMethod(IPv4Config::Off);
    }
    else {
        obj.setMethod(IPv4Config::Off);
    }
    return argument;
}

const QVariantMap &operator<<(QVariantMap &argument, const IPv4Config &obj)
{
    argument.insert(PropertyAddress, QVariant::fromValue(obj.address()));

    argument.insert(PropertyNetMask, QVariant::fromValue(obj.mask()));

    argument.insert(PropertyGateway, QVariant::fromValue(obj.gateway()));

    QString method;
    switch (obj.method()) {
    case IPv4Config::Dhcp:
        method = AttributeDhcp;
        break;
    case IPv4Config::Manual:
        method = AttributeManual;
        break;
    case IPv4Config::Off:
        method = AttributeOff;
        break;
    default:
        break;
    }
    argument.insert(PropertyMethod, QVariant::fromValue(method));
    return argument;
}

const QVariantMap &operator>>(const QVariantMap &argument, IPv6Config &obj)
{
    obj.setAddress(argument[PropertyAddress].toString());
    obj.setGateway(argument[PropertyGateway].toString());
    obj.setPrefixLength(argument[PropertyPrefixLength].toInt());
    QString method = argument[PropertyMethod].toString();
    if (method == AttributeAuto) {
        obj.setMethod(IPv6Config::Auto);
    }
    else if (method == AttributeManual) {
        obj.setMethod(IPv6Config::Manual);
    }
    else if (method == AttributeOff) {
        obj.setMethod(IPv6Config::Off);
    }
    QString privacy = argument[PropertyPrivacy].toString();

    if (privacy == AttributeDisabled) {
        obj.setPrivacy(IPv6Config::Disabled);
    }
    else if (privacy == AttributeEnabled) {
        obj.setPrivacy(IPv6Config::Enabled);
    }
    else if (privacy == AttributePreferred) {
        obj.setPrivacy(IPv6Config::Preferred);
    }
    return argument;
}

const QVariantMap &operator<<(QVariantMap &argument, const IPv6Config &obj)
{
    argument.insert(PropertyAddress, QVariant::fromValue(obj.address()));
    argument.insert(PropertyGateway, QVariant::fromValue(obj.address()));
    argument.insert(PropertyPrefixLength, QVariant::fromValue(obj.prefixLength()));

    QString method;
    switch (obj.method()) {
    case IPv6Config::Auto:
        method = AttributeAuto;
        break;
    case IPv6Config::Manual:
        method = AttributeManual;
        break;
    case IPv6Config::Off:
        method = AttributeOff;
        break;
    default:
        break;
    }
    argument.insert(PropertyMethod, QVariant::fromValue(method));

    QString privacy;
    switch (obj.privacy()) {
    case IPv6Config::Disabled:
        method = AttributeDisabled;
        break;
    case IPv6Config::Enabled:
        method = AttributeEnabled;
        break;
    case IPv6Config::Preferred:
        method = AttributePreferred;
        break;
    default:
        break;
    }
    argument.insert(PropertyPrivacy, QVariant::fromValue(privacy));

    return argument;
}


const QVariantMap &operator>>(const QVariantMap &argument, ProxyConfig &obj)
{
    QString method = argument[PropertyMethod].toString();
    if (method == AttributeDirect) {
        obj.setMethod(ProxyConfig::Direct);
    }
    else if (method == AttributeAuto) {
        obj.setMethod(ProxyConfig::Auto);
    }
    else if (method == AttributeManual) {
        obj.setMethod(ProxyConfig::Manual);
    }

    QString urlStr = argument[PropertyUrl].toString();
    obj.setUrl(QUrl(urlStr));

    obj.setServers(argument[PropertyServers].toStringList());
    obj.setExcludes(argument[PropertyExcludes].toStringList());

    return argument;
}

const QVariantMap &operator<<(QVariantMap &argument, const ProxyConfig &obj)
{
    QString method;
    switch (obj.method()) {
    case ProxyConfig::Direct:
        method = AttributeDirect;
        break;
    case ProxyConfig::Auto:
        method = AttributeAuto;
        break;
    case ProxyConfig::Manual:
        method = AttributeManual;
        break;
    default:
        break;
    }
    argument.insert(PropertyMethod, QVariant::fromValue(method));
    argument.insert(PropertyUrl, QVariant::fromValue(obj.url().toString()));
    argument.insert(PropertyServers, QVariant::fromValue(obj.servers()));
    argument.insert(PropertyServers, QVariant::fromValue(obj.excludes()));

    return argument;
}

const QString &operator>>(const QString &argument, NetworkType &obj)
{
    if (argument == AttributeEthernet) {
        obj.setType(NetworkType::Wired);
    }
    else if (argument == AttributeWifi) {
        obj.setType(NetworkType::Wifi);
    }
    else {
        obj.setType(NetworkType::Unknown);
    }

    return argument;
}

const QString &operator>>(const QString &argument, NetworkState &obj)
{
    if (argument == AttributeIdle) {
        obj.setState(NetworkState::Idle);
    }
    else if (argument == AttributeFailure) {
        obj.setState(NetworkState::Failure);
    }
    else if (argument == AttributeAssociation) {
        obj.setState(NetworkState::Association);
    }
    else if (argument == AttributeConfiguration) {
        obj.setState(NetworkState::Configuration);
    }
    else if (argument == AttributeReady) {
        obj.setState(NetworkState::Ready);
    }
    else if (argument == AttributeDisconnect) {
        obj.setState(NetworkState::Disconnect);
    }
    else if (argument == AttributeOneline) {
        obj.setState(NetworkState::Online);
    }
    else {
        obj.setState(NetworkState::Undefined);
    }
    return argument;
}

NetworkServicePrivate::NetworkServicePrivate(const QString& aPath, NetworkService *parent) :
    QObject(parent)
    ,q_ptr(parent)
    ,m_path(aPath)
{
    qDBusRegisterMetaType<ConnmanMap>();
    qDBusRegisterMetaType<ConnmanMapList>();

    m_service = new NetConnmanServiceInterface("net.connman", m_path,
                                                  QDBusConnection::systemBus(), this);

    connect(m_service, SIGNAL(PropertyChanged(QString,QDBusVariant)),
            this, SLOT(updateProperty(QString,QDBusVariant)));

    QDBusPendingReply<QVariantMap> reply = m_service->GetProperties();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(propertiesUpdated(QDBusPendingCallWatcher*)));
}

void NetworkServicePrivate::setupIpv6Config()
{
    QVariantMap param;
    param << m_ipv6config;
    if (m_service)
        m_service->SetProperty(PropertyIPv6Config, QDBusVariant(QVariant(param)));
}

void NetworkServicePrivate::setupNameserversConfig()
{
    QVariant param = QVariant::fromValue(m_nameserverConfig.stringList());
    if (m_service)
        m_service->SetProperty(PropertyNameserversConfig, QDBusVariant(QVariant(param)));
}

void NetworkServicePrivate::setupDomainsConfig()
{
    QVariant param = QVariant::fromValue(m_domainsConfig.stringList());
    if (m_service)
        m_service->SetProperty(PropertyDomainsConfig, QDBusVariant(QVariant(param)));
}

void NetworkServicePrivate::setupProxyConfig()
{
    QVariantMap param;
    param << m_proxyConfig;
    if (m_service)
        m_service->SetProperty(PropertyProxyConfig, QDBusVariant(QVariant(param)));
}

void NetworkServicePrivate::setupIpv4Config()
{
    QVariantMap param;
    param << m_ipv4config;
    if (m_service)
        m_service->SetProperty(PropertyIPv4Config, QDBusVariant(QVariant(param)));
}


void NetworkServicePrivate::propertiesUpdated(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<QVariantMap> reply = *call;
    call->deleteLater();

    if (!reply.isError())
        setupConfiguration(reply.value());
}

void NetworkServicePrivate::updateProperty(const QString &name, const QDBusVariant &value)
{
    updateProperty(name, value.variant());
}

void NetworkServicePrivate::updateProperty(const QString& key, const QVariant& val)
{
    Q_Q(NetworkService);

    if (key == PropertyIPv4) {
        QVariantMap value = qdbus_cast<QVariantMap>(val);
        value >> m_ipv4config;
        emit q->ipv4Changed();
    }
    else if (key == PropertyIPv6) {
        QVariantMap value = qdbus_cast<QVariantMap>(val);
        value >> m_ipv6config;
        emit q->ipv6Changed();
    }
    else if (key == PropertyNameservers) {
        QStringList value = qdbus_cast<QStringList>(val);
        m_nameserverConfig.setStringList(value);
        emit q->nameserversChanged();
    }
    else if (key == PropertyDomains) {
        QStringList value = qdbus_cast<QStringList>(val);
        m_domainsConfig.setStringList(value);
        emit q->domainsChanged();
    }
    else if (key == PropertyProxy) {
        QVariantMap value = qdbus_cast<QVariantMap>(val);
        value >> m_proxyConfig;
        emit q->proxyChanged();
    }
    else if (key == PropertyState) {
        QString value = qdbus_cast<QString>(val);
        value >> m_state;
        emit q->stateChanged();
    }
    else if (key == PropertyName) {
        m_name = qdbus_cast<QString>(val);
        emit q->nameChanged();
    }
    else if (key == PropertyType) {
        QString value = qdbus_cast<QString>(val);
        value >> m_type;
    }
}

void NetworkServicePrivate::setupConfiguration(const QVariantMap &properties)
{
    QVariantMap::const_iterator it = properties.constBegin(), end = properties.constEnd();
    for ( ; it != end; ++it) {
       updateProperty(it.key(), it.value());
    }
}

NetworkTechnology::NetworkTechnology(const QString& path, const QVariantMap& properties, QObject* parent)
    : NetworkItem(parent)
    {
        m_technology = new NetConnmanTechnologyInterface("net.connman", path,
                                                      QDBusConnection::systemBus(), this);
        connect(m_technology, SIGNAL(PropertyChanged(QString,QDBusVariant)),
               this, SLOT(updateProperty(QString,QDBusVariant)));

        updateProperty(PropertyName, properties[PropertyName]);
        updateProperty(PropertyType, properties[PropertyType]);
        updateProperty(PropertyConnected, properties[PropertyConnected]);
        updateProperty(PropertyPowered, properties[PropertyPowered]);
    }

void NetworkTechnology::updateProperty(const QString &name, const QDBusVariant &value)
{
    updateProperty(name, value.variant());
}

void NetworkTechnology::updateProperty(const QString &name, const QVariant &value)
{
    if (name == PropertyName) {
        m_name = qdbus_cast<QString>(value);
    }
    else if (name == PropertyType) {
        qdbus_cast<QString>(value) >> m_type;
        emit typeChanged();
    }
    else if (name == PropertyConnected) {
        bool connected = qdbus_cast<bool>(value);
        if (connected)
            m_state.setState(NetworkState::Online);
        else
            m_state.setState(NetworkState::Disconnect);
        emit stateChanged();
    }
    else if (name == PropertyPowered) {
        m_powered = qdbus_cast<bool>(value);
        emit poweredChanged();
    }
}


NetworkSettingsManagerPrivate::NetworkSettingsManagerPrivate(NetworkSettingsManager *parent)
    :QObject(parent)
    ,q_ptr(parent)
{
    m_manager = new NetConnmanManagerInterface("net.connman", "/",
            QDBusConnection::systemBus(), this);

    if (m_manager->isValid()) {
        //List technologies
        QDBusPendingReply<ConnmanMapList> reply = m_manager->GetTechnologies();
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
        connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                this, SLOT(getTechnologiesFinished(QDBusPendingCallWatcher*)));

        reply = m_manager->GetServices();
        watcher = new QDBusPendingCallWatcher(reply, this);
        connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
               this, SLOT(getServicesFinished(QDBusPendingCallWatcher*)));
    }
    else {
        delete m_manager;
        m_manager = NULL;
    }
}

void NetworkSettingsManagerPrivate::getServicesFinished(QDBusPendingCallWatcher *watcher)
{
    Q_Q(NetworkSettingsManager);
    QDBusPendingReply<ConnmanMapList> reply = *watcher;
    watcher->deleteLater();
    if (reply.isError())
        return;

    foreach (const ConnmanMap &object, reply.value()) {
        const QString servicePath = object.objectPath.path();
        NetworkService *service = new NetworkService(servicePath, this);
        m_services.append(service);

        //Update status property
        QString n = qdbus_cast<QString>(object.propertyMap[PropertyName]);
        QString t = qdbus_cast<QString>(object.propertyMap[PropertyType]);
        QString s = qdbus_cast<QString>(object.propertyMap[PropertyState]);

        NetworkType type;
        t >> type;
        NetworkState state;
        s >> state;

        foreach (NetworkItem* item, m_networksModel.getModel()) {
            if (item->name() == n && item->type() == type.type()) {
                item->setState(state.state());
            }
        }
    }
    emit q->servicesChanged();
}

void NetworkSettingsManagerPrivate::getTechnologiesFinished(QDBusPendingCallWatcher *watcher)
{
    Q_Q(NetworkSettingsManager);
    QDBusPendingReply<ConnmanMapList> reply = *watcher;
    watcher->deleteLater();
    if (reply.isError())
        return;

    foreach (const ConnmanMap &object, reply.value()) {
        NetworkTechnology *item = new NetworkTechnology(object.objectPath.path(), object.propertyMap, this);

        //Wired to the top
        if (item->type() == NetworkType::Wired)
            m_networksModel.insert(0, item);
        else if (item->type() == NetworkType::Wifi) {
            connect(item, &NetworkItem::poweredChanged, q, &NetworkSettingsManager::wifiPoweredChanged);
            m_networksModel.append(item);
        }
        emit q->networksChanged();
    }
}

bool NetworkSettingsManagerPrivate::wifiPowered()
{
    foreach (NetworkItem* item, m_networksModel.getModel()) {
        if (item->type() == NetworkType::Wifi) {
            return item->powered();
        }
    }
    return false;
}

void NetworkSettingsManagerPrivate::setWifiPowered(const bool powered)
{
    foreach (NetworkItem* item, m_networksModel.getModel()) {
        if (item->type() == NetworkType::Wifi) {
            NetworkTechnology* technology = qobject_cast<NetworkTechnology*>(item);
            if (technology) {
                NetConnmanTechnologyInterface *connmantech = technology->getTechnology();
                connmantech->SetProperty(PropertyPowered, QDBusVariant(QVariant(powered)));
            }
        }
    }
}
