/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
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
import QtQuick
import QtDeviceUtilities.NetworkSettings
import DeviceUtilities.SettingsUI
import DeviceUtilities.QtButtonImageProvider

ListView {
    id: list
    clip: true
    property var connectingService: null
    property bool retryConnectAfterIdle: false

    focus: true
    boundsBehavior: Flickable.DragOverBounds
    model: NetworkSettingsManager.services

    function connectBySsid() {
        passphraseEnter.showSsid = true
        passphraseEnter.visible = true
    }

    delegate: Item {
        id: networkDelegate
        width: list.width
        height: list.height * 0.15
        Column {
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width * 0.5
            Text {
                id: networkName
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: Globals.subTitleFontSize
                font.family: Globals.appFont
                color: connected ? Globals.buttonGreenColor : "white"
                text: (type === NetworkSettingsType.Wired) ? name + " (" + entry["id"] + ")" : name
            }
            Row {
                id: ipRow
                height: networkDelegate.height * 0.275 * opacity
                spacing: networkDelegate.width * 0.0075
                Item {
                    width: Globals.margin(list.width)
                    height: 1
                }
                Text {
                    id: ipAddressLabel
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("IP Address:")
                    color: connected ? Globals.buttonGreenColor : "white"
                    font.pixelSize: Globals.valueFontSize
                    font.family: Globals.appFont
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
                Text {
                    id: ipAddress
                    width: root.width * 0.15
                    height: parent.height
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    color: connected ? Globals.buttonGreenColor : "white"
                    text: connected ? NetworkSettingsManager.services.itemFromRow(index).ipv4.address
                                        : (NetworkSettingsManager.services.itemFromRow(index).state === NetworkSettingsState.Idle) ?
                                        qsTr("Not connected") : qsTr("Connecting")
                    font.pixelSize: Globals.valueFontSize
                    font.family: Globals.appFont
                    font.styleName: connected ? "SemiBold" : "Regular"
                }
            }
        }
        QtButton {
            id: connectButton
            fontFamily: Globals.appFont
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            fillColor: connected ? Globals.buttonGrayColor : Globals.buttonGreenColor
            borderColor: "transparent"
            text: connected ? qsTr("DISCONNECT") : qsTr("CONNECT")
            enabled: true
            onClicked: {
                if (connected) {
                    NetworkSettingsManager.services.itemFromRow(index).disconnectService();
                } else {
                    list.connectingService = NetworkSettingsManager.services.itemFromRow(index)
                    if (list.connectingService) {
                        passphraseEnter.extraInfo = "";
                        list.connectingService.connectService();
                        list.connectingService.stateChanged.connect(connectingServiceStateChange);
                    }
                }
            }
        }
        Rectangle {
            id: delegateBottom
            width: networkDelegate.width
            color: Globals.borderColor
            height: 2
            anchors.bottom: networkDelegate.bottom
            anchors.horizontalCenter: networkDelegate.horizontalCenter
        }
        Behavior on height { NumberAnimation { duration: 200} }
    }

    Connections {
        target: NetworkSettingsManager.userAgent
        function onShowUserCredentialsInput() {
            passphraseEnter.visible = true;
        }
    }

    // Popup for entering passphrase
    PassphraseEnter {
        id: passphraseEnter
        parent: list.parent
        visible: false
    }

    function connectingServiceStateChange() {
        if (connectingService !== null) {
            if (connectingService.state === NetworkSettingsState.Failure) {
                // If authentication failed, request connection again. That will
                // initiate new passphrase request.
                retryConnectAfterIdle = true
            } else if (connectingService.state === NetworkSettingsState.Ready) {
                // If connection succeeded, we no longer have service connecting
                connectingService = null;
                retryConnectAfterIdle = false;
            } else if (connectingService.state === NetworkSettingsState.Idle) {
                if (retryConnectAfterIdle) {
                    passphraseEnter.extraInfo = qsTr("Invalid passphrase");
                    connectingService.connectService();
                }
                retryConnectAfterIdle = false;
            }
        }
    }
}
