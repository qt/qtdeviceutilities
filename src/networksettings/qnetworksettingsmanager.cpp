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
#include "qnetworksettingsmanager.h"
#include "qnetworksettingsservice.h"
#include "qnetworksettingsservicemodel.h"
#include "qnetworksettingsinterfacemodel.h"
#include "qnetworksettingsmanager_p.h"
#include "qnetworksettingsuseragent.h"
#include <QStringListModel>

QT_BEGIN_NAMESPACE

QNetworkSettingsManager::QNetworkSettingsManager(QObject *parent) :
   QObject(parent)
   ,d_ptr(new QNetworkSettingsManagerPrivate(this))
{
}

QNetworkSettingsServiceFilter *QNetworkSettingsManager::services()
{
    Q_D(QNetworkSettingsManager);
    return d->serviceFilter();
}

QNetworkSettingsInterfaceModel *QNetworkSettingsManager::interfaces()
{
    Q_D(QNetworkSettingsManager);
    return d->interfaceModel();
}

QNetworkSettingsService* QNetworkSettingsManager::service(const QString& name, const int type)
{
    Q_D(QNetworkSettingsManager);

    foreach (QNetworkSettingsService* service, d->serviceModel()->getModel()) {
        if (service->name() == name && service->type() == type) {
            return service;
        }
    }
    return NULL;
}

void QNetworkSettingsManager::connectBySsid(const QString &name, const QString &passphrase)
{
    Q_D(QNetworkSettingsManager);
    QNetworkSettingsUserAgent* agent = userAgent();
    if (agent)
        agent->setSsidAndPassphrase(name, passphrase);
    d->connectBySsid(name);
}

void QNetworkSettingsManager::clearConnectionState()
{
    Q_D(QNetworkSettingsManager);
    d->clearConnectionState();
    QNetworkSettingsUserAgent* agent = userAgent();
    if (agent)
        agent->clearConnectionState();
}

void QNetworkSettingsManager::tryNextConnection()
{
    Q_D(QNetworkSettingsManager);
    d->tryNextConnection();
}

void QNetworkSettingsManager::clearCurrentWifiConnection(QNetworkSettingsService* service)
{
    Q_D(QNetworkSettingsManager);
    QNetworkSettingsService *currentService = d->currentWifiConnection();
    if (service == currentService) {
        d->setCurrentWifiConnection(nullptr);
        emit currentWifiConnectionChanged();
    }
}

void QNetworkSettingsManager::setCurrentWifiConnection(QNetworkSettingsService* service)
{
    Q_D(QNetworkSettingsManager);
    QNetworkSettingsService *currentService = d->currentWifiConnection();
    d->setCurrentWifiConnection(service);
    if (service != currentService)
        emit currentWifiConnectionChanged();
}

QNetworkSettingsService* QNetworkSettingsManager::currentWifiConnection()
{
    Q_D(QNetworkSettingsManager);
    return d->currentWifiConnection();
}

void QNetworkSettingsManager::setUserAgent(QNetworkSettingsUserAgent *agent)
{
    Q_D(QNetworkSettingsManager);
    d->setUserAgent(agent);
    connect(agent, &QNetworkSettingsUserAgent::requestNextConnection,
            this, &QNetworkSettingsManager::tryNextConnection);
}

QNetworkSettingsUserAgent* QNetworkSettingsManager::userAgent()
{
    Q_D(QNetworkSettingsManager);
    return d->userAgent();
}

QT_END_NAMESPACE
