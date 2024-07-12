// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick

Item {
    id: root
    property bool scanning: false
    property int signalStrength: 100
    property bool connected: false

    onSignalStrengthChanged: {
        sprite.visible = true;

        if (signalStrength < 10) {
            sprite.visible = false;
        }
        else if (signalStrength < 30) {
            sprite.currentFrame = 0;
        }
        else if (signalStrength < 60) {
            sprite.currentFrame = 1;
        }
        else if (signalStrength < 80) {
            sprite.currentFrame = 2;
        }
        else if (signalStrength <= 100) {
            sprite.currentFrame = 3;
        }
    }

    Image {
        id: sprite
        property int currentFrame: 0
        anchors.fill: parent
        source: "icons/Wifi_lightgray_2x.png"
        clip: true

        Timer {
            id: scanningTimer
            running: scanning
            interval: 250
            repeat: true
            onTriggered: {
                if (sprite.currentFrame < 4)
                    sprite.currentFrame++
                else
                    sprite.currentFrame = 0
            }
        }

        Image {
            height: parent.height
            width: parent.width * 4
            source: "icons/WifiAnim_black_2x.png"
            x: -parent.currentFrame * width / 4
        }
    }
}
