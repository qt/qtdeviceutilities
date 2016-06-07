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

Rectangle {
    id: hand
    objectName: "hand"
    x: root.height / 2 - width / 2
    y: root.height / 2 - height + root.handOffset
    color: editMode ? "#d6d6d6" : "#5caa15"
    width: root.width * 0.080
    height: root.height / 2 * 0.65 + root.handOffset
    antialiasing: true
    property alias angle: handRotation.angle
    property alias value: angleBinding.value
    transform: Rotation {
        id: handRotation
        origin.x: Math.round(hand.width / 2)
        origin.y: Math.round(hand.height - root.handOffset)
        Behavior on angle {
            enabled: !mouseArea.pressed
            SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
        }

        Binding on angle {
            id: angleBinding
            when: !mouseArea.pressed
        }
    }
    Item {
        objectName: "handle"
        anchors.verticalCenter: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.width * 3
        width: parent.width * 3
        visible: editMode
        Rectangle {
            anchors.fill: parent
            anchors.margins: parent.width * .1
            radius: width / 2
            color: "#5caa15"
        }
    }
}
