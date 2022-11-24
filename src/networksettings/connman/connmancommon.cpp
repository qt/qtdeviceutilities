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
    if (argument == AttributeEthernet) {
        obj.setType(QNetworkSettingsType::Wired);
    }
    else if (argument == AttributeWifi) {
        obj.setType(QNetworkSettingsType::Wifi);
    }
    else if (argument == AttributeBluetooth) {
        obj.setType(QNetworkSettingsType::Bluetooth);
    }
    else {
        obj.setType(QNetworkSettingsType::Unknown);
    }

    return argument;
}

const QString &operator>>(const QString &argument, QNetworkSettingsState &obj)
{
    if (argument == AttributeIdle) {
        obj.setState(QNetworkSettingsState::Idle);
    }
    else if (argument == AttributeFailure) {
        obj.setState(QNetworkSettingsState::Failure);
    }
    else if (argument == AttributeAssociation) {
        obj.setState(QNetworkSettingsState::Association);
    }
    else if (argument == AttributeConfiguration) {
        obj.setState(QNetworkSettingsState::Configuration);
    }
    else if (argument == AttributeReady) {
        obj.setState(QNetworkSettingsState::Ready);
    }
    else if (argument == AttributeDisconnect) {
        obj.setState(QNetworkSettingsState::Disconnect);
    }
    else if (argument == AttributeOneline) {
        obj.setState(QNetworkSettingsState::Online);
    }
    else {
        obj.setState(QNetworkSettingsState::Undefined);
    }
    return argument;
}

QT_END_NAMESPACE
