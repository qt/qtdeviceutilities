// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ItemDelegate {
    id: root
    autoExclusive: true
    property bool connect: connected
    contentItem: Item {
        width: root.width

        Label {
            id: text
            leftPadding: root.spacing
            anchors.left: parent.left
            anchors.top:parent.top
            anchors.right: signalMonitor.left
            anchors.bottom:parent.bottom
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            text: entry["name"]
        }
        WifiSignalMonitor {
            id: signalMonitor
            anchors.right: parent.right
            height: parent.height
            width: height
            signalStrength: entry.wirelessConfig["signalStrength"]
            connected: connected
        }
    }
}
