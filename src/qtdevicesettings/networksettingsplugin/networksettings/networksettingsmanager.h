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
#ifndef NETWORKSETTINGSMANAGER_H
#define NETWORKSETTINGSMANAGER_H

#include "networkconfig.h"
#include <QObject>
#include <QQmlListProperty>
#include <QStringListModel>

QT_FORWARD_DECLARE_CLASS(NetworkSettingsManagerPrivate)
QT_FORWARD_DECLARE_CLASS(NetworkServicePrivate)
QT_FORWARD_DECLARE_CLASS(NetworkNetworkTypePrivate)

class NetworkService : public QObject
{
    Q_OBJECT
    Q_ENUMS(StateTypes)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(NetworkState::States state READ state NOTIFY stateChanged)
    Q_PROPERTY(NetworkType::Types type READ type NOTIFY typeChanged)
    Q_PROPERTY(IPv4Config* ipv4 READ ipv4 NOTIFY ipv4Changed)
    Q_PROPERTY(IPv6Config* ipv6 READ ipv6 NOTIFY ipv6Changed)
    Q_PROPERTY(ProxyConfig* proxy READ proxy  NOTIFY proxyChanged)
    Q_PROPERTY(QAbstractItemModel* domains READ domains NOTIFY domainsChanged)
    Q_PROPERTY(QAbstractItemModel* nameservers READ nameservers NOTIFY nameserversChanged)
public:
    explicit NetworkService(const QString& aServiceId, QObject* parent = 0);

    QString name() const;
    NetworkState::States state();
    NetworkType::Types type();
    IPv4Config* ipv4();
    IPv6Config* ipv6();
    ProxyConfig* proxy();
    QAbstractItemModel* domains();
    QAbstractItemModel* nameservers();

    Q_INVOKABLE void setAutoConnect(const bool autoconnect);
    Q_INVOKABLE void setupIpv4Config();
    Q_INVOKABLE void setupIpv6Config();
    Q_INVOKABLE void setupNameserversConfig();
    Q_INVOKABLE void setupDomainsConfig();
    Q_INVOKABLE void setupProxyConfig();

Q_SIGNALS:
    void nameChanged();
    void stateChanged();
    void typeChanged();
    void proxyChanged();
    void ipv4Changed();
    void ipv6Changed();
    void domainsChanged();
    void nameserversChanged();

protected:
    NetworkServicePrivate *d_ptr;

    Q_DISABLE_COPY(NetworkService)
    Q_DECLARE_PRIVATE(NetworkService)
};

class NetworkSettingsManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(StateTypes NetworkTypeTypes)
    Q_PROPERTY(QQmlListProperty<NetworkService> services READ services NOTIFY servicesChanged)
    Q_PROPERTY(QAbstractItemModel* networks READ networks NOTIFY networksChanged)
    Q_PROPERTY(bool wifiPowered READ wifiPowered WRITE setWifiPowered NOTIFY wifiPoweredChanged)
public:
    explicit NetworkSettingsManager(QObject* parent = 0);
    QQmlListProperty<NetworkService> services();

    QAbstractItemModel* networks();

    Q_INVOKABLE NetworkService* getService(const QString& aName, const int aType);

    bool wifiPowered();
    void setWifiPowered(const bool power);

Q_SIGNALS:
    void servicesChanged();
    void wifiPoweredChanged();
    void networksChanged();

protected:
    NetworkSettingsManagerPrivate *d_ptr;

private:
    Q_DISABLE_COPY(NetworkSettingsManager)
    Q_DECLARE_PRIVATE(NetworkSettingsManager)
};

#endif // NETWORKSETTINGSMANAGER_H
