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
    id: top

    GroupBox {
        id: groupBox
        title: qsTr("Devices")
        anchors.fill: parent

        ListView {
            id: mainList
            anchors.fill: parent
            opacity: BtDevice.scanning ? .5 : 1.0
            interactive: !BtDevice.scanning
            clip: true
            model: BtDevice.deviceModel

            function getIcon(deviceType) {
                switch (deviceType) {
                case BtDeviceItem.Computer:
                    return "../icons/Laptop_qt_1x.png"
                case BtDeviceItem.Headphones:
                    return "../icons/Headphones_qt_1x.png"
                case BtDeviceItem.Microphone:
                    return "../icons/Microphone_qt_1x.png"
                case BtDeviceItem.Mouse:
                    return "../icons/Mouse_qt_1x.png"
                case BtDeviceItem.Keyboard:
                    return "../icons/Keyboard_qt_1x.png"
                default:
                    return "../icons/Bluetooth_qt_1x.png"
                }
            }

            delegate: Item {
                id: btDelegate
                width: parent.width
                height: expanded || connected ?  column.height + 10 : bttext.height + 10
                property bool expanded: false;

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (!connected)
                            btDelegate.expanded = !btDelegate.expanded
                    }
                }
                Image {
                    id: bticon
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.topMargin: 4
                    source: mainList.getIcon(deviceType)
                }
                Column {
                    id: column
                    anchors.left: bticon.right
                    anchors.leftMargin: 10
                    anchors.right: connectButton.left
                    anchors.rightMargin: 10

                    Label {
                        id: bttext
                        text: name
                    }
                    Label {
                        id: details
                        opacity: btDelegate.expanded || connected ? 1 : 0.0
                        text: address
                        Behavior on opacity { NumberAnimation { duration: 200} }
                    }
                }
                Button {
                    id: connectButton
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    visible: (expanded || connected) && !BtDevice.scanning
                    text: connected ? qsTr("Disconnect") : qsTr("Connect")
                    onClicked : connected ? BtDevice.requestDisconnect(address) : BtDevice.requestPairing(address);
                }
                Behavior on height { NumberAnimation { duration: 200} }
            }
            focus: true
        }
    }
    BusyIndicator {
        anchors.centerIn: parent
        running: BtDevice.scanning
    }
}
