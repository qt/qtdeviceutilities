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
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtDeviceUtilities.BluetoothSettings 1.0

Item {
    id: root
    property string title: qsTr("Bluetooth Settings")

    Text {
        id: bluetoothText
        anchors.top: parent.top
        anchors.left: parent.left
        fontSizeMode: Text.Fit
        minimumPixelSize: 1
        font.pixelSize: parent.height * 0.05
        color: "white"
        text: qsTr("Bluetooth")
        font.family: appFont
        font.styleName: "Bold"
    }

    Rectangle {
        id: btmLine
        anchors.top: bluetoothText.bottom
        anchors.topMargin: parent.height * 0.025
        anchors.left: bluetoothText.left
        width: parent.width * 0.275
        height: parent.height * 0.005
    }
    Row {
        id: bluetoothRow
        anchors.top: btmLine.bottom
        anchors.topMargin: parent.height * 0.05
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height * 0.2
        spacing: parent.width * 0.025
        Text {
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: parent.height * 0.2
            color: "white"
            text: "Bluetooth"
            font.family: appFont
            font.styleName: "Bold"
        }
        CustomSwitch {
            anchors.verticalCenter: parent.verticalCenter
            indicatorWidth: root.width * 0.25
            indicatorHeight: root.height * 0.1

            checked: BtDevice.powered
            onCheckedChanged: BtDevice.powered = checked
        }

        Text {
            id: scanningText
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height * 0.2
            color: "white"
            text: "Scanning"
            font.family: appFont
            opacity: BtDevice.scanning ? 1.0 : 0.0
            visible: opacity > 0
            Behavior on opacity {
                NumberAnimation {duration: 150}
            }

            Timer {
                id: scanningTimer
                interval: 1000
                repeat: true
                running: BtDevice.scanning

                onTriggered: {
                    if (scanningText.text.indexOf("...") !== -1)
                        scanningText.text = "Scanning"
                    else
                        scanningText.text += "."
                }
            }
        }

        Connections {
            target: BtDevice
            onPoweredChanged: {
                if (BtDevice.powered)
                    BtDevice.scanning = true
            }
        }
    }

    Discovery {
        id: discovery
        anchors.top: bluetoothRow.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height * 0.085
        visible: BtDevice.powered
    }
}
