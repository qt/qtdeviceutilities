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
#include "qnetworksettingsinterface.h"
#include "qnetworksettingsinterfacemodel.h"
#include "qnetworksettingsmanager_p.h"
#include <QStringListModel>

QNetworkSettingsManager::QNetworkSettingsManager(QObject *parent) :
   QObject(parent)
   ,d_ptr(new QNetworkSettingsManagerPrivate(this))
{
}

QNetworkSettingsServiceFilter *QNetworkSettingsManager::services()
{
    Q_D(QNetworkSettingsManager);
    return d->m_serviceFilter;
}

QNetworkSettingsInterfaceModel *QNetworkSettingsManager::interfaces()
{
    Q_D(QNetworkSettingsManager);
    return &d->m_interfaceModel;
}

QNetworkSettingsService* QNetworkSettingsManager::service(const QString& name, int type)
{
    Q_D(QNetworkSettingsManager);

    foreach (QNetworkSettingsService* service, d->m_serviceModel->getModel()) {
        if (service->name() == name && service->type() == type) {
            return service;
        }
    }
    return nullptr;
}

QNetworkSettingsInterface* QNetworkSettingsManager::interface(int type, int instance)
{
    Q_D(QNetworkSettingsManager);
    int matchingInstance = 0;

    foreach (QNetworkSettingsInterface* interface, d->m_interfaceModel.getModel()) {
        if (interface->type() == type) {
            if (matchingInstance == instance) {
                return interface;
            }
            matchingInstance++;
        }
    }
    return nullptr;
}

void QNetworkSettingsManager::setUserAgent(QNetworkSettingsUserAgent *agent)
{
    Q_D(QNetworkSettingsManager);
    d->setUserAgent(agent);
}

QNetworkSettingsUserAgent* QNetworkSettingsManager::userAgent()
{
    Q_D(QNetworkSettingsManager);
    return d->userAgent();
}
