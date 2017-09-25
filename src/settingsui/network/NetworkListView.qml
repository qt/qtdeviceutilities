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
import QtQml 2.0
import QtDeviceUtilities.NetworkSettings 1.0
import QtDeviceUtilities.QtButtonImageProvider 1.0
import QtQuick.Controls 2.1

ListView {
    id: list
    clip: true
    property bool connecting: false

    Component.onCompleted: NetworkSettingsManager.services.type = NetworkSettingsType.Unknown;
    model: NetworkSettingsManager.services

    delegate: Item {
        id: networkDelegate
        width: list.width
        height: expanded ? list.height * 0.15 + list.height * 0.05 : list.height * 0.15
        property bool expanded: false
        MouseArea {
            anchors.fill: parent
            onClicked: networkDelegate.expanded = !networkDelegate.expanded
        }
        Rectangle {
            id: img
            height: parent.height * 0.6
            width: height
            anchors.left: parent.left
            anchors.leftMargin: parent.width * 0.075
            anchors.verticalCenter: parent.verticalCenter
        }
        Column {
            anchors.left: img.right
            anchors.leftMargin: parent.width * 0.025
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width * 0.5
            Text {
                id: networkName
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: list.height * 0.06
                color: connected ? "#41cd52" : "white"
                text: name
                font.family: appFont
                font.styleName: connected ? "SemiBold" : "Regular"
            }
            Row {
                id: ipRow
                height: networkDelegate.height * 0.275 * opacity
                opacity: networkDelegate.expanded ? 1 : 0.0
                visible: opacity > 0
                spacing: networkDelegate.width * 0.0075
                Behavior on opacity { NumberAnimation { duration: 200} }
                Text {
                    id: ipAddressLabel
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("IP Address:")
                    color: connected ? "#41cd52" : "white"
                    font.pixelSize: height * 0.8
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    font.family: appFont
                    font.styleName: connected ? "SemiBold" : "Regular"
                }
                Text {
                    id: ipAddress
                    width: root.width * 0.15
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    color: connected ? "#41cd52" : "white"
                    visible: ipRow.opacity > 0
                    text: connected ? NetworkSettingsManager.services.itemFromRow(index).ipv4.address : qsTr("Not connected")
                    font.family: appFont
                    font.styleName: connected ? "SemiBold" : "Regular"
                }
            }
        }
        QtButton {
            id: connectButton
            height: list.height * 0.1
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            fillColor: connected ? "#9d9faa" : "#41cd52"
            borderColor: "transparent"
            text: connected ? qsTr("DISCONNECT") : qsTr("CONNECT")
            opacity: expanded || connected ? 1.0 : 0.0
            visible: opacity > 0.0
            Behavior on opacity { NumberAnimation { duration: 200 } }
            onClicked: {
                if (connected) {
                    NetworkSettingsManager.services.itemFromRow(index).disconnectService();
                } else {
                    var service = NetworkSettingsManager.services.itemFromRow(index)
                    if (service) {
                        list.connecting = true
                        service.connectService();
                    }
                }
            }
        }
        Rectangle {
            id: delegateBottom
            width: networkDelegate.width
            color: "#9d9faa"
            height: 2
            anchors.bottom: networkDelegate.bottom
            anchors.horizontalCenter: networkDelegate.horizontalCenter
        }
        Behavior on height { NumberAnimation { duration: 200} }

        Connections {
            target: NetworkSettingsManager.userAgent
            onShowUserCredentialsInput : {
                settingsLoader.source = "qrc:/network/PassphraseEnter.qml"
                list.connecting = false
            }
            onError: {
                list.connecting = false
                console.log("ERROR OCCURRED")
            }
        }
    }
    focus: true
}
