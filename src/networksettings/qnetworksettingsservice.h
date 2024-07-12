// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QNETWORKSETTINGSSERVICE_H
#define QNETWORKSETTINGSSERVICE_H

#include <QtCore/qobject.h>
#include <QtNetworkSettings/qnetworksettings.h>

QT_BEGIN_NAMESPACE

class QNetworkSettingsServicePrivate;
class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QNetworkSettingsState::State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QNetworkSettingsType::Type type READ type NOTIFY typeChanged)
    Q_PROPERTY(QNetworkSettingsIPv4* ipv4 READ ipv4 NOTIFY ipv4Changed)
    Q_PROPERTY(QNetworkSettingsIPv6* ipv6 READ ipv6 NOTIFY ipv6Changed)
    Q_PROPERTY(QNetworkSettingsProxy* proxy READ proxy  NOTIFY proxyChanged)
    Q_PROPERTY(QNetworkSettingsWireless* wirelessConfig READ wirelessConfig NOTIFY wirelessChanged)
    Q_PROPERTY(QAbstractItemModel* domains READ domains NOTIFY domainsChanged)
    Q_PROPERTY(QAbstractItemModel* nameservers READ nameservers NOTIFY nameserversChanged)
    Q_PROPERTY(bool autoConnect READ autoConnect WRITE setAutoConnect NOTIFY autoConnectChanged)
public:
    explicit QNetworkSettingsService(const QString& aServiceId, QObject* parent = nullptr);

    QString id() const;
    QString name() const;
    QNetworkSettingsState::State state();
    QNetworkSettingsType::Type type();
    QNetworkSettingsIPv4* ipv4();
    QNetworkSettingsIPv6* ipv6();
    QNetworkSettingsProxy* proxy();
    QAbstractItemModel* domains();
    QAbstractItemModel* nameservers();
    QNetworkSettingsWireless* wirelessConfig();
    void setPlaceholderState(bool placeholderState);
    bool placeholderState() const;

    Q_INVOKABLE void setAutoConnect(bool autoconnect);
    Q_INVOKABLE bool autoConnect() const;
    Q_INVOKABLE void setupIpv4Config();
    Q_INVOKABLE void setupIpv6Config();
    Q_INVOKABLE void setupNameserversConfig();
    Q_INVOKABLE void setupDomainsConfig();
    Q_INVOKABLE void setupNetworkSettingsProxy();
    //Wireless config
    Q_INVOKABLE void connectService();
    void doConnectService();
    Q_INVOKABLE void disconnectService();
    Q_INVOKABLE void removeService();
Q_SIGNALS:
    void autoConnectChanged();
    void nameChanged();
    void stateChanged();
    void typeChanged();
    void proxyChanged();
    void ipv4Changed();
    void ipv6Changed();
    void domainsChanged();
    void nameserversChanged();
    void wirelessChanged();
    void connectionStateCleared();
    void serviceConnected(QNetworkSettingsService* service);
    void serviceDisconnected(QNetworkSettingsService* service);
protected:
    QNetworkSettingsServicePrivate *d_ptr;

    Q_DISABLE_COPY(QNetworkSettingsService)
    Q_DECLARE_PRIVATE(QNetworkSettingsService)
};

QT_END_NAMESPACE

#endif // QNETWORKSETTINGSSERVICE_H
