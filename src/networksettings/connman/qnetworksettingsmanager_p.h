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
#include <QtDBus>
#include <QMap>
#include "connmancommon.h"
#include "qnetworksettingsmanager.h"
#include "qnetworksettingsinterfacemodel.h"

// Automatically generated class in global namespace
class NetConnmanManagerInterface;
QT_FORWARD_DECLARE_CLASS(QNetworkSettingsServiceModel)
QT_FORWARD_DECLARE_CLASS(QNetworkSettingsServiceFilter)
QT_FORWARD_DECLARE_CLASS(QNetworkSettingsService)

QT_BEGIN_NAMESPACE

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
    void setCurrentWifiConnection(QNetworkSettingsService *connection) {m_currentWifiConnection = connection;}
    QNetworkSettingsService* currentWifiConnection() const {return m_currentWifiConnection;}
    void setCurrentWiredConnection(QNetworkSettingsService *connection) {m_currentWiredConnection = connection;}
    QNetworkSettingsService* currentWiredConnection() const {return m_currentWiredConnection;}
    QString usbEthernetInternetProtocolAddress();
    QString usbVirtualEthernetLinkProtocol();
    bool hasUsbEthernetProtocolConfiguration();
    void setUsbVirtualEthernetLinkProtocol(const QString &protocol);

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
    void writeUsbEthernetProtocolToFileContent(QByteArray &fileContent, const QString &protocol);
    static QByteArray readQdbdFileContent();
    static QByteArray readUsbEthernetProtocolLine();

protected:
    QNetworkSettingsInterfaceModel m_interfaceModel;
    QNetworkSettingsServiceModel *m_serviceModel;
    QMap<QString, QNetworkSettingsService*> m_unnamedServices;
    QMap<QString, QNetworkSettingsService*> m_unnamedServicesForSsidConnection;
    QNetworkSettingsServiceFilter *m_serviceFilter;
private:
    NetConnmanManagerInterface *m_manager;
    QNetworkSettingsUserAgent *m_agent;
    QDBusServiceWatcher *m_serviceWatcher;
    QString m_currentSsid;
    QNetworkSettingsService *m_currentWifiConnection;
    QNetworkSettingsService *m_currentWiredConnection;
    bool m_initialized;
};

QT_END_NAMESPACE

#endif // QNETWORKSETTINGSMANAGERPRIVATE_H
