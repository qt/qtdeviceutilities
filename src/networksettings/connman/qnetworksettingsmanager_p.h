/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
******************************************************************************/
#ifndef QNETWORKSETTINGSMANAGERPRIVATE_H
#define QNETWORKSETTINGSMANAGERPRIVATE_H

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

#include <QObject>
#include <QMap>
#include <QPointer>
#include <QtDBus/QDBusServiceWatcher>
#include "connmancommon.h"
#include "qnetworksettingsmanager.h"
#include "qnetworksettingsinterfacemodel.h"
#include "connman_manager_interface.h"

QT_BEGIN_NAMESPACE

class QDBusObjectPath;
class QDBusPendingCallWatcher;
class QNetworkSettingsService;
class QNetworkSettingsServiceModel;
class QNetworkSettingsServiceFilter;

class QNetworkSettingsManagerPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(QNetworkSettingsManager)
public:
    explicit QNetworkSettingsManagerPrivate(QNetworkSettingsManager *parent);
    QNetworkSettingsManager *q_ptr;
    void setUserAgent(QNetworkSettingsUserAgent *agent);
    QNetworkSettingsUserAgent *userAgent() const {return m_agent;}
    QNetworkSettingsInterfaceModel* interfaceModel() {return &m_interfaceModel;}
    QNetworkSettingsServiceModel* serviceModel() const {return m_serviceModel;}
    QNetworkSettingsServiceFilter* serviceFilter() const {return m_serviceFilter;}
    void connectBySsid(const QString &name);
    void clearConnectionState();
    void tryNextConnection();
    void setCurrentWifiConnection(QNetworkSettingsService *connection);
    QNetworkSettingsService* currentWifiConnection() const;
    void setCurrentWiredConnection(QNetworkSettingsService *connection);
    QNetworkSettingsService* currentWiredConnection() const;

public slots:
    void getServicesFinished(QDBusPendingCallWatcher *watcher);
    void getTechnologiesFinished(QDBusPendingCallWatcher *watcher);
    void requestInput(const QString& service, const QString& type);
    void onServicesChanged(ConnmanMapStructList changed, const QList<QDBusObjectPath> &removed);
    void serviceReady();
    void onConnmanServiceRegistered(const QString &serviceName);
    void onTechnologyAdded(const QDBusObjectPath &technology, const QVariantMap &properties);
    void onTechnologyRemoved(const QDBusObjectPath &technology);

private:
    bool initialize();
    void handleNewService(const QString &servicePath);

protected:
    QNetworkSettingsInterfaceModel m_interfaceModel;
    QNetworkSettingsServiceModel *m_serviceModel;
    QMap<QString, QNetworkSettingsService*> m_unknownServices;
    QMap<QString, QNetworkSettingsService*> m_unnamedServices;
    QMap<QString, QNetworkSettingsService*> m_unnamedServicesForSsidConnection;
    QNetworkSettingsServiceFilter *m_serviceFilter;
private:
    NetConnmanManagerInterface *m_manager;
    QNetworkSettingsUserAgent *m_agent;
    QDBusServiceWatcher *m_serviceWatcher;
    QString m_currentSsid;
    QPointer<QNetworkSettingsService> m_currentWifiConnection;
    QPointer<QNetworkSettingsService> m_currentWiredConnection;
    bool m_initialized;
};

QT_END_NAMESPACE

#endif // QNETWORKSETTINGSMANAGERPRIVATE_H
