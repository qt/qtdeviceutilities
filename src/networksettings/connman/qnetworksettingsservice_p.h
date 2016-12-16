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
#ifndef QNETWORKSETTINGSSERVICEPRIVATE_H
#define QNETWORKSETTINGSSERVICEPRIVATE_H

#include "qnetworksettingsservice.h"
#include "qnetworksettings.h"

class QDBusPendingCallWatcher;
class QDBusVariant;
class NetConnmanServiceInterface;

class QNetworkSettingsServicePrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(QNetworkSettingsService)
public:
    QNetworkSettingsServicePrivate(const QString& aPath, QNetworkSettingsService *parent = Q_NULLPTR);

    QNetworkSettingsService *q_ptr;
private slots:
    void propertiesUpdated(QDBusPendingCallWatcher *call);
    void updateProperty(const QString &name, const QDBusVariant &value);

private:
    void setupConfiguration(const QVariantMap &properties);
    void updateProperty(const QString& key, const QVariant& value);

protected:
    void setAutoConnect(const bool autoconnect);
    void setupIpv4Config();
    void setupIpv6Config();
    void setupNameserversConfig();
    void setupDomainsConfig();
    void setupQNetworkSettingsProxy();
    void connectService();
    void disconnectService();

    QString m_id;
    QString m_name;
    QNetworkSettingsState m_state;
    QNetworkSettingsIPv4 m_ipv4config;
    QNetworkSettingsIPv6 m_ipv6config;
    QNetworkSettingsAddressModel m_domainsConfig;
    QNetworkSettingsAddressModel m_nameserverConfig;
    QNetworkSettingsProxy m_proxyConfig;
    QNetworkSettingsWireless m_wifiConfig;
    QNetworkSettingsType m_type;
    NetConnmanServiceInterface *m_service;
};

#endif // QNETWORKSETTINGSSERVICEPRIVATE_H
