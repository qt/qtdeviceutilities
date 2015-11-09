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
#include "networkconfig.h"
#include "networksettingsmanager.h"

#include <qqml.h>
#include <QQmlEngine>

template <typename T>
QObject *instance(QQmlEngine *engine, QJSEngine *) {
    T *t = new T(engine);
    t->setObjectName(T::staticMetaObject.className());
    return t;
}

void NetworksettingspluginPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("com.theqtcompany.settings.network"));
    qmlRegisterUncreatableType<NetworkService>(uri, 1, 0, "NetworkService", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<IPv4Config>(uri, 1, 0, "IPv4Config", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<IPv6Config>(uri, 1, 0, "IPv6Config", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<ProxyConfig>(uri, 1, 0, "ProxyConfig", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<NetworkType>(uri, 1, 0, "NetworkType", "Cannot be instantiated directly.");
    qmlRegisterSingletonType<NetworkSettingsManager>(uri, 1, 0, "NetworkSettingsManager", &instance<NetworkSettingsManager>);
}


