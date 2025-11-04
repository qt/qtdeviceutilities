// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef NETWORKSETTINGSPLUGIN_PLUGIN_H
#define NETWORKSETTINGSPLUGIN_PLUGIN_H

#include <QtQml/qqml.h>
#include <QtNetworkSettings/QtNetworkSettings>

QT_BEGIN_NAMESPACE

struct NetworkSettingsServiceForeign
{
    Q_GADGET
    QML_FOREIGN(QNetworkSettingsService)
    QML_NAMED_ELEMENT(NetworkService)
    QML_UNCREATABLE("Cannot be instantiated directly.")
    QML_ADDED_IN_VERSION(1, 0)
};

struct NetworkSettingsInterfaceForeign
{
    Q_GADGET
    QML_FOREIGN(QNetworkSettingsInterface)
    QML_NAMED_ELEMENT(NetworkInterface)
    QML_UNCREATABLE("Cannot be instantiated directly.")
    QML_ADDED_IN_VERSION(1, 0)
};

struct NetworkSettingsIPv4Foreign
{
    Q_GADGET
    QML_FOREIGN(QNetworkSettingsIPv4)
    QML_NAMED_ELEMENT(NetworkSettingsIPv4)
    QML_UNCREATABLE("Cannot be instantiated directly.")
    QML_ADDED_IN_VERSION(1, 0)
};

struct NetworkSettingsIPv6Foreign
{
    Q_GADGET
    QML_FOREIGN(QNetworkSettingsIPv6)
    QML_NAMED_ELEMENT(NetworkSettingsIPv6)
    QML_UNCREATABLE("Cannot be instantiated directly.")
    QML_ADDED_IN_VERSION(1, 0)
};

struct NetworkSettingsProxyForeign
{
    Q_GADGET
    QML_FOREIGN(QNetworkSettingsProxy)
    QML_NAMED_ELEMENT(NetworkSettingsProxy)
    QML_UNCREATABLE("Cannot be instantiated directly.")
    QML_ADDED_IN_VERSION(1, 0)
};

struct NetworkSettingsWirelessForeign
{
    Q_GADGET
    QML_FOREIGN(QNetworkSettingsWireless)
    QML_NAMED_ELEMENT(NetworkSettingsWireless)
    QML_UNCREATABLE("Cannot be instantiated directly.")
    QML_ADDED_IN_VERSION(1, 0)
};

struct NetworkSettingsTypeForeign
{
    Q_GADGET
    QML_FOREIGN(QNetworkSettingsType)
    QML_NAMED_ELEMENT(NetworkSettingsType)
    QML_UNCREATABLE("Cannot be instantiated directly.")
    QML_ADDED_IN_VERSION(1, 0)
};

struct NetworkSettingsStateForeign
{
    Q_GADGET
    QML_FOREIGN(QNetworkSettingsState)
    QML_NAMED_ELEMENT(NetworkSettingsState)
    QML_UNCREATABLE("Cannot be instantiated directly.")
    QML_ADDED_IN_VERSION(1, 0)
};

class QNetworkSettingsManagerForeign
{
    Q_GADGET
    QML_FOREIGN(QNetworkSettingsManager)
    QML_NAMED_ELEMENT(NetworkSettingsManager)
    QML_SINGLETON
    QML_ADDED_IN_VERSION(2, 0)
};

QT_END_NAMESPACE

#endif // NETWORKSETTINGSPLUGIN_PLUGIN_H

