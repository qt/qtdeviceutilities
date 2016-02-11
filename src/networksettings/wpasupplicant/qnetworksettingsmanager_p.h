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
#ifndef QNETWORKSETTINGSMANAGERPRIVATE_H
#define QNETWORKSETTINGSMANAGERPRIVATE_H

#include <QObject>
#include "qnetworksettings.h"
#include "qnetworksettingsmanager.h"
#include "qnetworksettingsinterfacemodel.h"
#include "qnetworksettingsservicemodel.h"
#include "qwificontroller_p.h"

class WpaSupplicantService;

class QNetworkSettingsManagerPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(QNetworkSettingsManager)
public:
    explicit QNetworkSettingsManagerPrivate(QNetworkSettingsManager *parent);
    virtual ~QNetworkSettingsManagerPrivate();
    QNetworkSettingsManager *q_ptr;
    void setUserAgent(QNetworkSettingsUserAgent *agent);
    void connectNetwork(const QString& ssid);
    void disconnectNetwork();
    QString call(const QString &command);
    bool checkedCall(const QString &command);
protected:
    bool event(QEvent *event);

private slots:
    void handleBackendStateChanged(QWifiController::BackendState backendState);
    void handleDhcpRequestFinished(const QString &status);
    void userInteractionReady(bool cancel);
    void updateLastError(const QString &error);
private:
    void setCurrentSSID(const QString &ssid);
    void handleConnected();
    void handleDisconneced();
    void handleAuthenticating(QWifiEvent *event);
    void updateNetworkState(QNetworkSettingsState::States networkState);
    void updateBackendState(QWifiController::BackendState backendState);
    void updateWifiState();
    void parseScanResults(const QString &results);
    WpaSupplicantService* networkForSSID(const QString& ssid);
    WpaSupplicantService* networkForSSID(const QString& ssid, int& pos);
    WpaSupplicantService* outOfRangeListContains(const QString& ssid);

    QNetworkSettingsInterfaceModel m_interfaceModel;
    QNetworkSettingsServiceModel m_serviceModel;
    QNetworkSettingsServiceModel m_outOfRangeServiceModel;
    QNetworkSettingsServiceFilter m_serviceFilter;
    QWifiController *m_wifiController;
    QNetworkSettingsUserAgent *m_agent; //Not owned

    QWifiController::BackendState m_backendState;
    QString m_currentSSID;
};


#endif // QNETWORKSETTINGSMANAGERPRIVATE_H
