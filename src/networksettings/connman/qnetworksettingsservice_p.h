// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QNETWORKSETTINGSSERVICEPRIVATE_H
#define QNETWORKSETTINGSSERVICEPRIVATE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qnetworksettingsservice.h"
#include "qnetworksettings.h"

// Automatically generated class in global namespace
class NetConnmanServiceInterface;

QT_BEGIN_NAMESPACE

class QDBusVariant;
class QDBusPendingCallWatcher;
class QNetworkSettingsServicePrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(QNetworkSettingsService)
public:
    QNetworkSettingsServicePrivate(const QString& aPath, QNetworkSettingsService *parent = nullptr);

    QNetworkSettingsService *q_ptr;
private slots:
    void propertiesUpdated(QDBusPendingCallWatcher *call);
    void updateProperty(const QString &name, const QDBusVariant &value);

private:
    void setupConfiguration(const QVariantMap &properties);
    void updateProperty(const QString& key, const QVariant& value);

protected:
    void setAutoConnect(bool autoconnect);
    bool autoConnect() const;
    void setupIpv4Config();
    void setupIpv6Config();
    void setupNameserversConfig();
    void setupDomainsConfig();
    void setupQNetworkSettingsProxy();
    void connectService();
    void disconnectService();
    void removeService();
    void setPlaceholderState(bool placeholderState);
    bool placeholderState() const;

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
    bool m_placeholderState = false;
    bool m_autoConnect = false;
};

QT_END_NAMESPACE

#endif // QNETWORKSETTINGSSERVICEPRIVATE_H
