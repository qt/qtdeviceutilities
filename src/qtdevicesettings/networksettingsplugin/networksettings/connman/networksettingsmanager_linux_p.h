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
#ifndef NETWORKSETTINGSMANAGERPRIVATE_H
#define NETWORKSETTINGSMANAGERPRIVATE_H

#include <QObject>
#include <QtDBus>
#include <QList>
#include <QtDBus/QDBusObjectPath>
#include <QtCore/QMetaType>
#include "networkaddressmodel.h"
#include "networksettingsmanager.h"
#include "networksmodel.h"

class NetConnmanManagerInterface;
class NetConnmanServiceInterface;
class NetworkService;
class NetConnmanTechnologyInterface;

struct ConnmanMap {
    QDBusObjectPath objectPath;
    QVariantMap propertyMap;
};

QDBusArgument &operator<<(QDBusArgument &argument, const ConnmanMap &obj);

const QDBusArgument &operator>>(const QDBusArgument &argument, ConnmanMap &obj);

typedef QList<ConnmanMap> ConnmanMapList;

Q_DECLARE_METATYPE( ConnmanMap )
Q_DECLARE_METATYPE( ConnmanMapList )

class NetworkServicePrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(NetworkService)
public:
    NetworkServicePrivate(const QString& aPath, NetworkService *parent=0);

    NetworkService *q_ptr;
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
    void setupProxyConfig();

    QString m_path;
    QString m_name;
    NetworkState m_state;
    IPv4Config m_ipv4config;
    IPv6Config m_ipv6config;
    NetworkAddressModel m_domainsConfig;
    NetworkAddressModel m_nameserverConfig;
    ProxyConfig m_proxyConfig;
    WifiConfig m_wifiConfig;
    NetworkType m_type;
    NetConnmanServiceInterface *m_service;
};

class NetworkTechnology : public NetworkItem
{
    Q_OBJECT
public:
    explicit NetworkTechnology(const QString& path, const QVariantMap& properties, QObject* parent=0);
    virtual ~NetworkTechnology() {

    }

    NetConnmanTechnologyInterface *getTechnology() {
        return m_technology;
    }

public slots:
    void updateProperty(const QString &name, const QDBusVariant &value);
protected:
    void updateProperty(const QString &name, const QVariant &value);

private:
    NetConnmanTechnologyInterface *m_technology;
};


class NetworkSettingsManagerPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(NetworkSettingsManager)
public:
    explicit NetworkSettingsManagerPrivate(NetworkSettingsManager *parent);
    NetworkSettingsManager *q_ptr;
    void setWifiPowered(const bool powered);
    bool wifiPowered();

public slots:
    void getServicesFinished(QDBusPendingCallWatcher *watcher);
    void getTechnologiesFinished(QDBusPendingCallWatcher *watcher);

protected:
    QList<NetworkService*> m_services;
    NetworksModel m_networksModel;
private:
    NetConnmanManagerInterface *m_manager;
};

#endif // NETWORKSETTINGSMANAGERPRIVATE_H
