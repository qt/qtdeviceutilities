// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include "connmancommon.h"

#include <QtNetworkSettings/qnetworksettings.h>

QT_BEGIN_NAMESPACE

//Conversion operations for datatypes
const QDBusArgument &operator>>(const QDBusArgument &argument, ConnmanMapStruct &obj)
{
    argument.beginStructure();
    argument >> obj.objectPath >> obj.propertyMap;
    argument.endStructure();
    return argument;
}

QDBusArgument &operator<<(QDBusArgument &argument, const ConnmanMapStruct &obj)
{
    argument.beginStructure();
    argument << obj.objectPath << obj.propertyMap;
    argument.endStructure();
    return argument;
}

const QString &operator>>(const QString &argument, QNetworkSettingsType &obj)
{
    if (argument == Q_AttributeEthernet) {
        obj.setType(QNetworkSettingsType::Wired);
    }
    else if (argument == Q_AttributeWifi) {
        obj.setType(QNetworkSettingsType::Wifi);
    }
    else if (argument == Q_AttributeBluetooth) {
        obj.setType(QNetworkSettingsType::Bluetooth);
    }
    else {
        obj.setType(QNetworkSettingsType::Unknown);
    }

    return argument;
}

const QString &operator>>(const QString &argument, QNetworkSettingsState &obj)
{
    if (argument == Q_AttributeIdle) {
        obj.setState(QNetworkSettingsState::Idle);
    }
    else if (argument == Q_AttributeFailure) {
        obj.setState(QNetworkSettingsState::Failure);
    }
    else if (argument == Q_AttributeAssociation) {
        obj.setState(QNetworkSettingsState::Association);
    }
    else if (argument == Q_AttributeConfiguration) {
        obj.setState(QNetworkSettingsState::Configuration);
    }
    else if (argument == Q_AttributeReady) {
        obj.setState(QNetworkSettingsState::Ready);
    }
    else if (argument == Q_AttributeDisconnect) {
        obj.setState(QNetworkSettingsState::Disconnect);
    }
    else if (argument == Q_AttributeOneline) {
        obj.setState(QNetworkSettingsState::Online);
    }
    else {
        obj.setState(QNetworkSettingsState::Undefined);
    }
    return argument;
}

QT_END_NAMESPACE
