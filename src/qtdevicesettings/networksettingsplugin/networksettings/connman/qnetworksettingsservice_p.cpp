/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
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
#include "qnetworksettingsservice_p.h"
#include "connman_service_interface.h"
#include "connmancommon.h"

const QString PropertyIPv4("IPv4");
const QString PropertyQNetworkSettingsIPv4("IPv4.Configuration");
const QString PropertyIPv6("IPv6");
const QString PropertyQNetworkSettingsIPv6("IPv6.Configuration");
const QString PropertyNameservers("Nameservers");
const QString PropertyNameserversConfig("Nameservers.Configuration");
const QString PropertyDomains("Domains");
const QString PropertyDomainsConfig("Domains.Configuration");
const QString PropertyProxy("Proxy");
const QString PropertyQNetworkSettingsProxy("Proxy.Configuration");
const QString PropertyAddress("Address");
const QString PropertyNetMask("Netmask");
const QString PropertyGateway("Gateway");
const QString PropertyPrefixLength("PrefixLength");
const QString PropertyMethod("Method");
const QString PropertyPrivacy("Privacy");
const QString PropertyUrl("Url");
const QString PropertyServers("Servers");
const QString PropertyExcludes("Excludes");
const QString PropertyStrength("Strength");
const QString PropertySecurity("Security");

const QString AttributeAuto("auto");
const QString AttributeDhcp("dhcp");
const QString AttributeManual("manual");
const QString AttributeOff("off");
const QString AttributeDisabled("disabled");
const QString AttributeEnabled("enabled");
const QString AttributePreferred("preferred");
const QString AttributeDirect("direct");
const QString AttributeNone("none");
const QString AttributeWep("wep");
const QString AttributePsk("psk");
const QString AttributeIeee("ieee8021x");
const QString AttributeWps("wps");
const QString AttributeInvalidKey("invalid-key");

const QVariantMap &operator>>(const QVariantMap &argument, QNetworkSettingsIPv4 &obj)
{
    obj.setAddress(argument[PropertyAddress].toString());
    obj.setMask(argument[PropertyNetMask].toString());
    obj.setGateway(argument[PropertyGateway].toString());

    QString method = argument[PropertyMethod].toString();

    if (method == AttributeDhcp) {
        obj.setMethod(QNetworkSettingsIPv4::Dhcp);
    }
    else if (method == AttributeManual) {
        obj.setMethod(QNetworkSettingsIPv4::Manual);
    }
    else if (method == AttributeOff) {
        obj.setMethod(QNetworkSettingsIPv4::Off);
    }
    else {
        obj.setMethod(QNetworkSettingsIPv4::Off);
    }
    return argument;
}

const QVariantMap &operator<<(QVariantMap &argument, const QNetworkSettingsIPv4 &obj)
{
    argument.insert(PropertyAddress, QVariant::fromValue(obj.address()));

    argument.insert(PropertyNetMask, QVariant::fromValue(obj.mask()));

    argument.insert(PropertyGateway, QVariant::fromValue(obj.gateway()));

    QString method;
    switch (obj.method()) {
    case QNetworkSettingsIPv4::Dhcp:
        method = AttributeDhcp;
        break;
    case QNetworkSettingsIPv4::Manual:
        method = AttributeManual;
        break;
    case QNetworkSettingsIPv4::Off:
        method = AttributeOff;
        break;
    default:
        break;
    }
    argument.insert(PropertyMethod, QVariant::fromValue(method));
    return argument;
}

const QVariantMap &operator>>(const QVariantMap &argument, QNetworkSettingsIPv6 &obj)
{
    obj.setAddress(argument[PropertyAddress].toString());
    obj.setGateway(argument[PropertyGateway].toString());
    obj.setPrefixLength(argument[PropertyPrefixLength].toInt());
    QString method = argument[PropertyMethod].toString();
    if (method == AttributeAuto) {
        obj.setMethod(QNetworkSettingsIPv6::Auto);
    }
    else if (method == AttributeManual) {
        obj.setMethod(QNetworkSettingsIPv6::Manual);
    }
    else if (method == AttributeOff) {
        obj.setMethod(QNetworkSettingsIPv6::Off);
    }
    QString privacy = argument[PropertyPrivacy].toString();

    if (privacy == AttributeDisabled) {
        obj.setPrivacy(QNetworkSettingsIPv6::Disabled);
    }
    else if (privacy == AttributeEnabled) {
        obj.setPrivacy(QNetworkSettingsIPv6::Enabled);
    }
    else if (privacy == AttributePreferred) {
        obj.setPrivacy(QNetworkSettingsIPv6::Preferred);
    }
    return argument;
}


const QVariantMap &operator<<(QVariantMap &argument, const QNetworkSettingsIPv6 &obj)
{
    argument.insert(PropertyAddress, QVariant::fromValue(obj.address()));
    argument.insert(PropertyGateway, QVariant::fromValue(obj.address()));
    argument.insert(PropertyPrefixLength, QVariant::fromValue(obj.prefixLength()));

    QString method;
    switch (obj.method()) {
    case QNetworkSettingsIPv6::Auto:
        method = AttributeAuto;
        break;
    case QNetworkSettingsIPv6::Manual:
        method = AttributeManual;
        break;
    case QNetworkSettingsIPv6::Off:
        method = AttributeOff;
        break;
    default:
        break;
    }
    argument.insert(PropertyMethod, QVariant::fromValue(method));

    QString privacy;
    switch (obj.privacy()) {
    case QNetworkSettingsIPv6::Disabled:
        method = AttributeDisabled;
        break;
    case QNetworkSettingsIPv6::Enabled:
        method = AttributeEnabled;
        break;
    case QNetworkSettingsIPv6::Preferred:
        method = AttributePreferred;
        break;
    default:
        break;
    }
    argument.insert(PropertyPrivacy, QVariant::fromValue(privacy));

    return argument;
}


const QVariantMap &operator>>(const QVariantMap &argument, QNetworkSettingsProxy &obj)
{
    QString method = argument[PropertyMethod].toString();
    if (method == AttributeDirect) {
        obj.setMethod(QNetworkSettingsProxy::Direct);
    }
    else if (method == AttributeAuto) {
        obj.setMethod(QNetworkSettingsProxy::Auto);
    }
    else if (method == AttributeManual) {
        obj.setMethod(QNetworkSettingsProxy::Manual);
    }

    QString urlStr = argument[PropertyUrl].toString();
    obj.setUrl(QUrl(urlStr));

    obj.setServers(argument[PropertyServers].toStringList());
    obj.setExcludes(argument[PropertyExcludes].toStringList());

    return argument;
}

const QVariantMap &operator<<(QVariantMap &argument, const QNetworkSettingsProxy &obj)
{
    QString method;
    switch (obj.method()) {
    case QNetworkSettingsProxy::Direct:
        method = AttributeDirect;
        break;
    case QNetworkSettingsProxy::Auto:
        method = AttributeAuto;
        break;
    case QNetworkSettingsProxy::Manual:
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

QNetworkSettingsServicePrivate::QNetworkSettingsServicePrivate(const QString& id, QNetworkSettingsService *parent) :
    QObject(parent)
    ,q_ptr(parent)
    ,m_id(id)
{
    qDBusRegisterMetaType<ConnmanMap>();
    qDBusRegisterMetaType<ConnmanMapList>();

    m_service = new NetConnmanServiceInterface("net.connman", m_id,
                                                  QDBusConnection::systemBus(), this);

    connect(m_service, SIGNAL(PropertyChanged(QString,QDBusVariant)),
            this, SLOT(updateProperty(QString,QDBusVariant)));

    QDBusPendingReply<QVariantMap> reply = m_service->GetProperties();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);

    connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
            this, SLOT(propertiesUpdated(QDBusPendingCallWatcher*)));
}

void QNetworkSettingsServicePrivate::setupIpv6Config()
{
    QVariantMap param;
    param << m_ipv6config;
    if (m_service)
        m_service->SetProperty(PropertyQNetworkSettingsIPv6, QDBusVariant(QVariant(param)));
}

void QNetworkSettingsServicePrivate::setupNameserversConfig()
{
    QVariant param = QVariant::fromValue(m_nameserverConfig.stringList());
    if (m_service)
        m_service->SetProperty(PropertyNameserversConfig, QDBusVariant(QVariant(param)));
}

void QNetworkSettingsServicePrivate::setupDomainsConfig()
{
    QVariant param = QVariant::fromValue(m_domainsConfig.stringList());
    if (m_service)
        m_service->SetProperty(PropertyDomainsConfig, QDBusVariant(QVariant(param)));
}

void QNetworkSettingsServicePrivate::setupQNetworkSettingsProxy()
{
    QVariantMap param;
    param << m_proxyConfig;
    if (m_service)
        m_service->SetProperty(PropertyQNetworkSettingsProxy, QDBusVariant(QVariant(param)));
}

void QNetworkSettingsServicePrivate::setupIpv4Config()
{
    QVariantMap param;
    param << m_ipv4config;
    if (m_service)
        m_service->SetProperty(PropertyQNetworkSettingsIPv4, QDBusVariant(QVariant(param)));
}

void QNetworkSettingsServicePrivate::connectService()
{
    if (m_service) {
        m_service->Connect();
    }
}

void QNetworkSettingsServicePrivate::disconnectService()
{
    if (m_service)
        m_service->Disconnect();

}

void QNetworkSettingsServicePrivate::propertiesUpdated(QDBusPendingCallWatcher *call)
{
    QDBusPendingReply<QVariantMap> reply = *call;
    call->deleteLater();

    if (!reply.isError())
        setupConfiguration(reply.value());
}

void QNetworkSettingsServicePrivate::updateProperty(const QString &name, const QDBusVariant &value)
{
    updateProperty(name, value.variant());
}

void QNetworkSettingsServicePrivate::updateProperty(const QString& key, const QVariant& val)
{
    Q_Q(QNetworkSettingsService);

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
    else if (key == PropertyStrength) {
        m_wifiConfig.setSignalStrength(val.toInt());
    }
    else if (key == PropertySecurity) {
        QStringList value = qdbus_cast<QStringList>(val);
        foreach (const QString str, value) {
            if (str ==  AttributeNone || str == AttributeWps) {
                m_wifiConfig.setSecurity(QNetworkSettingsWireless::None);
            }
            else if (str == AttributeWep) {
                m_wifiConfig.setSecurity(QNetworkSettingsWireless::WEP);
            }
            else if (str == AttributePsk) {
                m_wifiConfig.setSecurity(QNetworkSettingsWireless::WPA);
            }
            else if (str == AttributeIeee) {
                m_wifiConfig.setSecurity(QNetworkSettingsWireless::WPA2);
            }
        }
    }
}

void QNetworkSettingsServicePrivate::setupConfiguration(const QVariantMap &properties)
{
    QVariantMap::const_iterator it = properties.constBegin(), end = properties.constEnd();
    for ( ; it != end; ++it) {
       updateProperty(it.key(), it.value());
    }
}

