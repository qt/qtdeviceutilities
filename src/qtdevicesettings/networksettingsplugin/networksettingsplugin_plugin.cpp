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
#include "networksettingsplugin_plugin.h"
#include "qnetworksettings.h"
#include "qnetworksettingsmanager.h"
#include "qnetworksettingsservice.h"
#include "qnetworksettingsuseragent.h"

#include <qqml.h>
#include <QQmlEngine>
#include <QQmlContext>

void NetworksettingspluginPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("com.theqtcompany.settings.network"));
    qmlRegisterUncreatableType<QNetworkSettingsService>(uri, 1, 0, "NetworkService", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsIPv4>(uri, 1, 0, "NetworkSettingsIPv4", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsIPv6>(uri, 1, 0, "NetworkSettingsIPv6", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsProxy>(uri, 1, 0, "NetworkSettingsProxy", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsType>(uri, 1, 0, "NetworkSettingsType", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsState>(uri, 1, 0, "NetworkSettingsState", "Cannot be instantiated directly.");
}


void NetworksettingspluginPlugin::initializeEngine(QQmlEngine * engine, const char * uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("com.theqtcompany.settings.network"));
    QNetworkSettingsManager* networkManager = new QNetworkSettingsManager(engine);
    QNetworkSettingsUserAgent* userAgent = new QNetworkSettingsUserAgent(engine);
    networkManager->setUserAgent(userAgent);

    engine->rootContext()->setContextProperty("NetworkSettingsManager", networkManager);
    engine->rootContext()->setContextProperty("NetworkSettingsUserAgent", userAgent);
}
