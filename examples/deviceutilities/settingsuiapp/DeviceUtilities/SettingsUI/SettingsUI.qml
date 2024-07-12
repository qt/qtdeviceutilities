// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import DeviceUtilities.SettingsUI.Network

Rectangle {
    id: main
    anchors.fill: parent
    color: Globals.backgroundColor
    opacity: 0.97
    property int margin: Globals.margin(main.width)
    signal closed()

    NetworkSettings {
        anchors.margins: margin
    }
}
