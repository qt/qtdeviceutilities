/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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
import QtQuick 2.0
import QtDeviceUtilities.QtButtonImageProvider 1.0
import QtQuick.Controls 2.1

Switch {
    id: control

    property alias indicatorWidth: indicatorImg.width
    property alias indicatorHeight: indicatorImg.height

    indicator: Image {
        id: indicatorImg
        width: 200
        height: 75
        sourceSize: Qt.size(width, height)
        anchors.horizontalCenter: control.horizontalCenter
        y: parent.height / 2 - height / 2
        source: "image://QtButton/10/#848895/transparent"
        Text {
            id: offText
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.075
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            fontSizeMode: Text.Fit
            minimumPixelSize: 1
            font.pixelSize: parent.height * 0.55
            color: "#3b4155"
            text: "OFF"
            font.family: appFont
        }
        Text {
            id: onText
            anchors.right: parent.right
            anchors.rightMargin: parent.width * 0.1
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            fontSizeMode: Text.Fit
            minimumPixelSize: 1
            font.pixelSize: parent.height * 0.55
            color: "#3b4155"
            text: "ON"
            font.family: appFont
        }

        Binding {
            target: qtHandle
            property: "x"
            value: control.checked ? indicatorImg.width  - qtHandle.width - indicatorImg.width * 0.025 : indicatorImg.width * 0.025
            when: !mousearea.drag.active
        }

        MouseArea {
            anchors.fill: parent
            onClicked: control.checked = !control.checked
        }

        QtButton {
            id: qtHandle
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width * 0.475
            height: parent.height * 0.9
            fillColor: control.checked ? viewSettings.buttonGreenColor : viewSettings.buttonGrayColor
            text: control.checked ? "ON" : "OFF"
            borderColor: "transparent"
            Behavior on x {
                NumberAnimation { duration: 50 }
            }

            MouseArea {
                id: mousearea
                anchors.fill: parent
                drag.target: qtHandle
                drag.axis: Drag.XAxis
                drag.minimumX: indicatorImg.width * 0.005
                drag.maximumX: indicatorImg.width - width - indicatorImg.width * 0.005

                onClicked: control.checked = !control.checked

                onReleased: {
                    if (qtHandle.x > indicatorImg.width / 5 ) {
                        control.checked = true
                    } else {
                        control.checked = false
                    }
                }
            }
        }
    }
}
