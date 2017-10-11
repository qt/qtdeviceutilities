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
import QtDemoLauncher.QtButtonImageProvider 1.0

Item {
    id: top

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
            height: expanded || connected ? mainList.height * 0.175 + mainList.height * 0.05 : mainList.height * 0.175
            property bool expanded: false
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (!connected){
                        btDelegate.expanded = !btDelegate.expanded
                    }
                }
            }
            Image {
                id: bticon
                sourceSize.width: parent.width
                sourceSize.height: parent.height
                height: parent.height * 0.5
                width: height
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                source: mainList.getIcon(type)
            }
            Column {
                anchors.left: bticon.right
                anchors.leftMargin: parent.width * 0.025
                anchors.right: connectButton.left
                anchors.verticalCenter: parent.verticalCenter
                Label {
                    id: bttext
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: mainList.height * 0.06
                    text: name
                    color: connected ? "#41cd52" : "white"
                    font.family: appFont
                    font.styleName: connected ? "SemiBold" : "Regular"
                }
                Label {
                    id: details
                    height: btDelegate.height * 0.275 * opacity
                    opacity: btDelegate.expanded || connected ? 1 : 0.0
                    visible: opacity > 0
                    text: address
                    color: connected ? "#41cd52" : "white"
                    font.family: appFont
                    Behavior on opacity { NumberAnimation { duration: 200} }
                }
            }
            QtButton {
                id: connectButton
                height: mainList.height * 0.125
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                opacity: (expanded || connected) && !BtDevice.scanning ? 1.0 : 0.0
                visible: opacity > 0
                fillColor: connected ? "#9d9faa" : "#41cd52"
                borderColor: "transparent"
                text: connected ? qsTr("Disconnect") : qsTr("Connect")
                onClicked : connected ? BtDevice.requestDisconnect(address) : BtDevice.requestPairing(address);
                Behavior on opacity { NumberAnimation { duration: 200 } }
            }
            Behavior on height { NumberAnimation { duration: 200} }

            Rectangle {
                id: delegateBottom
                width: btDelegate.width
                color: "#9d9faa"
                height: 2
                anchors.bottom: btDelegate.bottom
                anchors.horizontalCenter: btDelegate.horizontalCenter
            }
        }
        focus: true
    }
}
