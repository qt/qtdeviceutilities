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
#include "qnetworksettingsmanager.h"
#include "qnetworksettingsservice.h"
#include "qnetworksettingsservicemodel.h"
#include "qnetworksettingsinterfacemodel.h"
#include "qnetworksettingsmanager_p.h"
#include <QStringListModel>

QNetworkSettingsManager::QNetworkSettingsManager(QObject *parent) :
   QObject(parent)
   ,d_ptr(new QNetworkSettingsManagerPrivate(this))
{
}

QAbstractItemModel* QNetworkSettingsManager::services()
{
    Q_D(QNetworkSettingsManager);
    return &d->m_serviceFilter;
}

QAbstractItemModel* QNetworkSettingsManager::interfaces()
{
    Q_D(QNetworkSettingsManager);
    return &d->m_interfaceModel;
}

QNetworkSettingsService* QNetworkSettingsManager::getService(const QString& name, const int type)
{
    Q_D(QNetworkSettingsManager);

    foreach (QNetworkSettingsService* service, d->m_serviceModel.getModel()) {
        if (service->name() == name && service->type() == type) {
            return service;
        }
    }
    return NULL;
}

void QNetworkSettingsManager::setUserAgent(QNetworkSettingsUserAgent *agent)
{
    Q_D(QNetworkSettingsManager);
    d->setUserAgent(agent);
}
