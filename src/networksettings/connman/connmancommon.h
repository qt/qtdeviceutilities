// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef CONNMANCOMMON_H
#define CONNMANCOMMON_H

#include <QtCore/QMetaType>
#include <QtDBus/QtDBus>

#define Q_PropertyAgentPath QStringLiteral("/ConnmanAgent")
#define Q_PropertyState QStringLiteral("State")
#define Q_PropertyName QStringLiteral("Name")
#define Q_PropertyType QStringLiteral("Type")
#define Q_PropertyConnected QStringLiteral("Connected")
#define Q_PropertyPowered QStringLiteral("Powered")
#define Q_PropertyAutoConnect QStringLiteral("AutoConnect")

#define Q_AttributeWifi QStringLiteral("wifi")
#define Q_AttributeEthernet QStringLiteral("ethernet")
#define Q_AttributeBluetooth QStringLiteral("bluetooth")
#define Q_AttributeIdle QStringLiteral("idle")
#define Q_AttributeFailure QStringLiteral("failure")
#define Q_AttributeAssociation QStringLiteral("association")
#define Q_AttributeConfiguration QStringLiteral("configuration")
#define Q_AttributeReady QStringLiteral("ready")
#define Q_AttributeDisconnect QStringLiteral("disconnect")
#define Q_AttributeOneline QStringLiteral("online")

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
