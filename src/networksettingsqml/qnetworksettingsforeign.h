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

