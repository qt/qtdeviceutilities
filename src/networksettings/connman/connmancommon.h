/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
******************************************************************************/
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
