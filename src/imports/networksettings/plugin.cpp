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
#include "plugin.h"
#include <qnetworksettings.h>
#include <qnetworksettingsmanager.h>
#include <qnetworksettingsservice.h>
#include <qnetworksettingsuseragent.h>

#include <qqml.h>
#include <QQmlEngine>
#include <QQmlContext>

template <typename T>
QObject *instance(QQmlEngine *engine, QJSEngine *) {
    T *t = new T(engine);
    t->setObjectName(T::staticMetaObject.className());
    return t;
}

void NetworksettingspluginPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("QtDeviceUtilities.NetworkSettings"));
    qmlRegisterUncreatableType<QNetworkSettingsService>(uri, 1, 0, "NetworkService", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsIPv4>(uri, 1, 0, "NetworkSettingsIPv4", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsIPv6>(uri, 1, 0, "NetworkSettingsIPv6", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsProxy>(uri, 1, 0, "NetworkSettingsProxy", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsType>(uri, 1, 0, "NetworkSettingsType", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsState>(uri, 1, 0, "NetworkSettingsState", "Cannot be instantiated directly.");

    qRegisterMetaType<QNetworkSettingsUserAgent*>("QNetworkSettingsUserAgent*");
    qmlRegisterSingletonType<QNetworkSettingsManager>(uri, 1, 0, "NetworkSettingsManager", &instance<QNetworkSettingsManager>);
}
