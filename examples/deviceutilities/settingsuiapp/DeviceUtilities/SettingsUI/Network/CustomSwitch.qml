// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtQuick.Controls
import DeviceUtilities.SettingsUI
import DeviceUtilities.QtButtonImageProvider

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
            font.family: Globals.appFont
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
            font.family: Globals.appFont
        }

        Binding {
            target: qtHandle
            property: "x"
            value: control.checked ? indicatorImg.width  - qtHandle.width - indicatorImg.width * 0.025 : indicatorImg.width * 0.025
            when: !mousearea.drag.active
        }

        QtButton {
            id: qtHandle
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width * 0.475
            height: parent.height * 0.9
            fillColor: control.checked ? Globals.buttonGreenColor : Globals.buttonGrayColor
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
