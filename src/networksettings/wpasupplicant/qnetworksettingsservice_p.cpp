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
#include "qnetworksettingsmanager_p.h"

QNetworkSettingsServicePrivate::QNetworkSettingsServicePrivate(const QString& bssid, QNetworkSettingsService *parent) :
    QObject(parent)
    ,q_ptr(parent)
    ,m_id(bssid)
{
    m_type.setType(QNetworkSettingsType::Wifi);
}

void QNetworkSettingsServicePrivate::setManager(QNetworkSettingsManagerPrivate *manager)
{
    m_manager = manager;
}

void QNetworkSettingsServicePrivate::connectService()
{
    m_manager->connectNetwork(m_name);
}

void QNetworkSettingsServicePrivate::disconnectService()
{
    m_manager->disconnectNetwork();
}

void QNetworkSettingsServicePrivate::setupIpv6Config()
{
    //Not supported
}

void QNetworkSettingsServicePrivate::setupNameserversConfig()
{
    //Not supported
}

void QNetworkSettingsServicePrivate::setupDomainsConfig()
{
    //Not supported
}

void QNetworkSettingsServicePrivate::setupQNetworkSettingsProxy()
{
    //Not supported
}

void QNetworkSettingsServicePrivate::setupIpv4Config()
{
    //Not supported
}

WpaSupplicantService::WpaSupplicantService(QNetworkSettingsManagerPrivate* manager, QObject* parent)
    :QNetworkSettingsService("", parent)
{
    Q_D(QNetworkSettingsService);
    d->setManager(manager);
}

void WpaSupplicantService::setId(const QString& aId)
{
    Q_D(QNetworkSettingsService);
    d->m_id = aId;
}

void WpaSupplicantService::setName(const QString& aName)
{
    Q_D(QNetworkSettingsService);
    d->m_name = aName;
    emit nameChanged();
}

void WpaSupplicantService::setFlags(const QString& aFlags)
{
    Q_D(QNetworkSettingsService);

    if (aFlags.contains("WPA-")) {
        d->m_wifiConfig.setSecurity(QNetworkSettingsWireless::WPA);
    }
    if (aFlags.contains("WPA2-")) {
        d->m_wifiConfig.setSecurity(QNetworkSettingsWireless::WPA2);
    }
    if (aFlags.contains("WEP-")) {
        d->m_wifiConfig.setSecurity(QNetworkSettingsWireless::WEP);
    }
}

void WpaSupplicantService::setState(QNetworkSettingsState::States aState)
{
    Q_D(QNetworkSettingsService);
    d->m_state.setState(aState);
    emit stateChanged();
}
