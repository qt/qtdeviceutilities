// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef CONNMANCOMMON_H
#define CONNMANCOMMON_H

#include <QtCore/QMetaType>
#include <QtDBus/QtDBus>

#define PropertyAgentPath QStringLiteral("/ConnmanAgent")
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

class QNetworkSettingsType;
class QNetworkSettingsState;

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
