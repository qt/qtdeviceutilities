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
#ifndef CONNMANCOMMON_H
#define CONNMANCOMMON_H

#include <QtCore/QMetaType>
#include <QtDBus>
#include <QtNetworkSettings/qnetworksettings.h>

#define AgentPath "/ConnmanAgent"
#define PropertyState QStringLiteral("State")
#define PropertyName QStringLiteral("Name")
#define PropertyType QStringLiteral("Type")
#define PropertyConnected QStringLiteral("Connected")
#define PropertyPowered QStringLiteral("Powered")
#define PropertyAutoConnect QStringLiteral("AutoConnect")

#define AttributeWifi QStringLiteral("wifi")
#define AttributeEthernet QStringLiteral("ethernet")
#define AttributeBluetooth QStringLiteral("bluetooth")
#define AttributeIdle QStringLiteral("idle")
#define AttributeFailure QStringLiteral("failure")
#define AttributeAssociation QStringLiteral("association")
#define AttributeConfiguration QStringLiteral("configuration")
#define AttributeReady QStringLiteral("ready")
#define AttributeDisconnect QStringLiteral("disconnect")
#define AttributeOneline QStringLiteral("online")

QT_BEGIN_NAMESPACE

struct ConnmanMapStruct {
    QDBusObjectPath objectPath;
    QVariantMap propertyMap;
};

QDBusArgument &operator<<(QDBusArgument &argument, const ConnmanMapStruct &obj);
const QDBusArgument &operator>>(const QDBusArgument &argument, ConnmanMapStruct &obj);

const QString &operator>>(const QString &argument, QNetworkSettingsType &obj);
const QString &operator>>(const QString  &argument, QNetworkSettingsState &obj);

typedef QList<ConnmanMapStruct> ConnmanMapStructList;

QT_END_NAMESPACE

Q_DECLARE_METATYPE( ConnmanMapStruct )
Q_DECLARE_METATYPE( ConnmanMapStructList )

#endif // CONNMANCOMMON_H
