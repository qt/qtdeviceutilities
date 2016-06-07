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

QNetworkSettingsService::QNetworkSettingsService(const QString& aServiceId, QObject* parent) :
   QObject(parent)
  ,d_ptr(new QNetworkSettingsServicePrivate(aServiceId, this))
{

}

QString QNetworkSettingsService::id() const
{
    Q_D(const QNetworkSettingsService);
    return d->m_id;
}

QString QNetworkSettingsService::name() const
{
    Q_D(const QNetworkSettingsService);
    return d->m_name;
}

void QNetworkSettingsService::setAutoConnect(const bool autoconnect)
{
    Q_UNUSED(autoconnect);
}

QNetworkSettingsState::States QNetworkSettingsService::state()
{
    Q_D(QNetworkSettingsService);
    return d->m_state.state();
}

QNetworkSettingsType::Types QNetworkSettingsService::type()
{
    Q_D(QNetworkSettingsService);
    return d->m_type.type();
}

QNetworkSettingsIPv4* QNetworkSettingsService::ipv4()
{
    Q_D(QNetworkSettingsService);
    return &d->m_ipv4config;
}

QNetworkSettingsIPv6* QNetworkSettingsService::ipv6()
{
    Q_D(QNetworkSettingsService);
    return &d->m_ipv6config;
}

QNetworkSettingsProxy* QNetworkSettingsService::proxy()
{
    Q_D(QNetworkSettingsService);
    return &d->m_proxyConfig;
}

QNetworkSettingsWireless* QNetworkSettingsService::wirelessConfig()
{
    Q_D(QNetworkSettingsService);
    return &d->m_wifiConfig;
}

QAbstractItemModel* QNetworkSettingsService::domains()
{
    Q_D(QNetworkSettingsService);
    return &d->m_domainsConfig;
}

QAbstractItemModel* QNetworkSettingsService::nameservers()
{
    Q_D(QNetworkSettingsService);
    return &d->m_nameserverConfig;
}

void QNetworkSettingsService::setupIpv4Config()
{
    Q_D(QNetworkSettingsService);
    d->setupIpv4Config();
}

void QNetworkSettingsService::setupIpv6Config()
{
    Q_D(QNetworkSettingsService);
    d->setupIpv6Config();
}

void QNetworkSettingsService::setupNameserversConfig()
{
    Q_D(QNetworkSettingsService);
    d->setupNameserversConfig();
}

void QNetworkSettingsService::setupDomainsConfig()
{
    Q_D(QNetworkSettingsService);
    d->setupDomainsConfig();
}

void QNetworkSettingsService::setupNetworkSettingsProxy()
{
    Q_D(QNetworkSettingsService);
    d->setupQNetworkSettingsProxy();
}

void QNetworkSettingsService::connectService()
{
    Q_D(QNetworkSettingsService);
    d->connectService();
}

void QNetworkSettingsService::disconnectService()
{
    Q_D(QNetworkSettingsService);
    d->disconnectService();
}
