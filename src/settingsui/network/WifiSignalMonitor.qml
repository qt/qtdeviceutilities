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
import QtQuick 2.6

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
        source: "../icons/Wifi_lightgray_2x.png"
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
            source: "../icons/WifiAnim_black_2x.png"
            x: -parent.currentFrame * width / 4
        }
    }
}
