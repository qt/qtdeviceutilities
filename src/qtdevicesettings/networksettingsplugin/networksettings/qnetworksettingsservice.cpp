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
