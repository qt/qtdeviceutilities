/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtBluetooth 5.2
import QtQuick.Controls.Styles.Flat 1.0 as Flat
import "../common"
import com.theqtcompany.settings.bluetooth 1.0

Item {
    id: top
    property BluetoothService currentService

    GroupBox {
        id: groupBox
        title: qsTr("Devices")
        width: parent.width

        ListView {
            id: mainList
            width: parent.width
            height: top.height

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

            model: BtDevice.deviceModel
            delegate: Rectangle {
                id: btDelegate
                width: parent.width
                height: Math.round(column.height + 10 * Flat.FlatStyle.scaleFactor)

                property bool expended: false;
                clip: true

                MouseArea {
                    anchors.fill: parent
                    onClicked: btDelegate.expended = !btDelegate.expended
                }

                RowLayout {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)

                    Image {
                       id: bticon
                       source: mainList.getIcon(type)
                    }

                    Column {
                        id: column
                        TextLabel {
                            id: bttext
                            text: name
                        }

                        TextLabel {
                            id: details
                            opacity: btDelegate.expended ? 1 : 0.0
                            text: address
                            Behavior on opacity {
                                NumberAnimation { duration: 200}
                            }
                        }
                    }
                }

                Button {
                    anchors.right: parent.right
                    anchors.rightMargin: Math.round(10 * Flat.FlatStyle.scaleFactor)
                    visible: !BtDevice.scanning
                    text: connected ? qsTr("Disconnect") : qsTr("Connect")

                    onClicked : connected ? BtDevice.requestDisconnect(address) : BtDevice.requestPairing(address);
                }

                Behavior on height { NumberAnimation { duration: 200} }

            }
            focus: true
        }
    }

    BusyIndicator {
        anchors.right: groupBox.right
        anchors.top: groupBox.top
        anchors.topMargin: Math.round(40 * Flat.FlatStyle.scaleFactor)
        anchors.rightMargin: Math.round(10 * Flat.FlatStyle.scaleFactor)
        height: Math.round(20 * Flat.FlatStyle.scaleFactor)
        width: Math.round(20 * Flat.FlatStyle.scaleFactor)
        running: BtDevice.scanning
     }
}
