// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QNETWORKSETTINGSMANAGER_H
#define QNETWORKSETTINGSMANAGER_H

#include <QtCore/qobject.h>
#include <QtNetworkSettings/qnetworksettingsglobal.h>

QT_BEGIN_NAMESPACE

class QNetworkSettingsService;
class QNetworkSettingsInterface;
class QNetworkSettingsUserAgent;
class QNetworkSettingsServiceFilter;
class QNetworkSettingsInterfaceModel;
class QNetworkSettingsManagerPrivate;
class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QNetworkSettingsServiceFilter* services READ services NOTIFY servicesChanged)
    Q_PROPERTY(QNetworkSettingsInterfaceModel* interfaces READ interfaces NOTIFY interfacesChanged)
    Q_PROPERTY(QNetworkSettingsUserAgent* userAgent READ userAgent CONSTANT)
    Q_PROPERTY(QNetworkSettingsService* currentWifiConnection READ currentWifiConnection NOTIFY currentWifiConnectionChanged)
    Q_PROPERTY(QNetworkSettingsService* currentWiredConnection READ currentWiredConnection NOTIFY currentWiredConnectionChanged)
    Q_MOC_INCLUDE("qnetworksettingsservicemodel.h")
    Q_MOC_INCLUDE("qnetworksettingsuseragent.h")

public:
    explicit QNetworkSettingsManager(QObject* parent = nullptr);
    QNetworkSettingsServiceFilter* services();
    QNetworkSettingsInterfaceModel* interfaces();
    void setUserAgent(QNetworkSettingsUserAgent *agent);
    QNetworkSettingsUserAgent* userAgent();

    Q_INVOKABLE QNetworkSettingsService* service(const QString& name, int type);
    Q_INVOKABLE void connectBySsid(const QString& name, const QString &passphrase);
    void clearConnectionState();
    void tryNextConnection();
    void clearCurrentConnection(QNetworkSettingsService* service);
    void setCurrentConnection(QNetworkSettingsService* service);
    QNetworkSettingsService* currentWifiConnection();
    QNetworkSettingsService* currentWiredConnection();
    Q_INVOKABLE QNetworkSettingsInterface* interface(int type, int instance);

Q_SIGNALS:
    void servicesChanged();
    void interfacesChanged();
    void currentWifiConnectionChanged();
    void currentWiredConnectionChanged();

protected:
    QNetworkSettingsManagerPrivate *d_ptr;

private:
    Q_DISABLE_COPY(QNetworkSettingsManager)
    Q_DECLARE_PRIVATE(QNetworkSettingsManager)
};

QT_END_NAMESPACE

#endif // QNETWORKSETTINGSMANAGER_H
