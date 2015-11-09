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
#include "networksettingsmanager.h"
#include "connman/networksettingsmanager_linux_p.h"
#include <QStringListModel>

NetworkService::NetworkService(const QString& aServiceId, QObject* parent) :
   QObject(parent)
  ,d_ptr(new NetworkServicePrivate(aServiceId, this))
{

}

QString NetworkService::name() const
{
    Q_D(const NetworkService);
    return d->m_name;
}

NetworkState::States NetworkService::state()
{
    Q_D(NetworkService);
    return d->m_state.state();
}

NetworkType::Types NetworkService::type()
{
    Q_D(NetworkService);
    return d->m_type.type();
}

IPv4Config* NetworkService::ipv4()
{
    Q_D(NetworkService);
    return &d->m_ipv4config;
}

IPv6Config* NetworkService::ipv6()
{
    Q_D(NetworkService);
    return &d->m_ipv6config;
}

ProxyConfig* NetworkService::proxy()
{
    Q_D(NetworkService);
    return &d->m_proxyConfig;
}

QAbstractItemModel* NetworkService::domains()
{
    Q_D(NetworkService);
    return &d->m_domainsConfig;
}

QAbstractItemModel* NetworkService::nameservers()
{
    Q_D(NetworkService);
    return &d->m_nameserverConfig;
}

void NetworkService::setupIpv4Config()
{
    Q_D(NetworkService);
    d->setupIpv4Config();
}

void NetworkService::setupIpv6Config()
{
    Q_D(NetworkService);
    d->setupIpv6Config();
}

void NetworkService::setupNameserversConfig()
{
    Q_D(NetworkService);
    d->setupNameserversConfig();
}

void NetworkService::setupDomainsConfig()
{
    Q_D(NetworkService);
    d->setupDomainsConfig();
}

void NetworkService::setupProxyConfig()
{
    Q_D(NetworkService);
    d->setupProxyConfig();
}

NetworkSettingsManager::NetworkSettingsManager(QObject *parent) :
   QObject(parent)
   ,d_ptr(new NetworkSettingsManagerPrivate(this))
{

}

QQmlListProperty<NetworkService> NetworkSettingsManager::services()
{
    Q_D(NetworkSettingsManager);
    return QQmlListProperty<NetworkService>(this, d->m_services);
}

QAbstractItemModel* NetworkSettingsManager::networks()
{
    Q_D(NetworkSettingsManager);
    return &d->m_networksModel;
}

NetworkService* NetworkSettingsManager::getService(const QString& aName, const int aType)
{
    Q_D(NetworkSettingsManager);

    foreach (NetworkService* service, d->m_services) {
        if (service->name() == aName && service->type() == aType) {
            return service;
        }
    }
    return NULL;
}


bool NetworkSettingsManager::wifiPowered()
{
    Q_D(NetworkSettingsManager);
    return d->wifiPowered();
}

void NetworkSettingsManager::setWifiPowered(const bool power)
{
    Q_D(NetworkSettingsManager);
    d->setWifiPowered(power);
}
