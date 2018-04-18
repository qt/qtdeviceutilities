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
import QtDeviceUtilities.NetworkSettings 1.0

Item {
    id: root
    anchors.fill: parent
    Component.onCompleted: NetworkSettingsManager.services.type = NetworkSettingsType.Wifi;
    property bool connecting: false

    Flickable {
        anchors.fill: parent
        contentHeight: content.height
        contentWidth: width

        GroupBox {
            title: qsTr("Wireless Settings")
            width: parent.width

            ColumnLayout {
                id: content
                spacing: 20
                width: parent.width

                RowLayout {
                    spacing: 10
                    id: enableSwitch
                    width: parent.width

                    Label {
                        Layout.preferredWidth: root.width * 0.382
                        Layout.alignment: Qt.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                        text: selectedInterface.powered ? qsTr("Wi-Fi ON") : qsTr("Wi-Fi OFF")
                    }
                    Switch {
                        checked: selectedInterface.powered
                        onCheckedChanged: {
                            selectedInterface.powered = checked
                            root.connecting = false
                            connectView.visible = false
                        }
                    }
                }
                RowLayout {
                    spacing: 10
                    width: parent.width

                    visible: selectedInterface.powered && networkSelection.count > 0
                    Label {
                        Layout.preferredWidth: root.width * 0.382
                        text: qsTr("Current network")
                        horizontalAlignment: Text.AlignRight
                        Layout.alignment: Qt.AlignVCenter

                    }
                    ComboBoxEntry {
                        id: networkSelection
                        model: NetworkSettingsManager.services

                        textRole: "name"
                        Layout.fillWidth: true
                        onActivated: {
                            if (index >= 0) {
                                connectView.visible = false

                                var service = model.itemFromRow(index)
                                if (service) {
                                    root.connecting = true
                                    service.connectService();
                                }
                            }
                        }

                        onCountChanged: {
                            if (count === 0) {
                                root.connecting = false
                                connectView.visible = false
                            }
                        }

                        Component.onCompleted: {
                            networkSelection.currentIndex = model.activeRow()
                        }

                        delegate: WifiSelectorDelegate {
                            width: networkSelection.width
                            onConnectChanged: if (connect) networkSelection.currentIndex = index
                        }
                    }
                }

                Row {
                    id: infoRow
                    spacing: 10
                    width: parent.width
                    visible: selectedInterface.powered && selectedInterface.state !== NetworkSettingsState.Online
                    Label {
                        id: scanningText
                        text: {
                            if (networkSelection.count == 0)
                                return qsTr("Searching for Wi-Fi networks...")
                            else if (root.connecting)
                                return qsTr("Connecting to the network...")
                            else
                                return ""
                        }
                        horizontalAlignment: Text.AlignLeft
                    }
                    WifiSignalMonitor {
                        id: scanningIcon
                        scanning: true
                        visible: scanningText.text !== ""
                        height: scanningText.height
                        width: height
                    }
                }

                GroupBox {
                    id: connectView
                    title: qsTr("Enter a passphrase")
                    visible: false
                    Layout.fillWidth: true
                    ColumnLayout {
                        width: parent.width

                        RowLayout {
                            id: errorView
                            visible: text.text !== ""
                            spacing: 10
                            property alias text: text.text

                            Image {
                                source: "../icons/Alert_yellow_1x.png"
                                Layout.alignment: Qt.AlignVCenter
                            }
                            Text {
                                id: text
                                color: "#face20"
                                text: ""
                                Layout.alignment: Qt.AlignVCenter
                            }
                        }
                        RowLayout {
                            spacing: 10
                            width: parent.width

                            Label {
                                text: qsTr("Passphrase:")
                                horizontalAlignment: Text.AlignRight
                                Layout.preferredWidth: root.width * 0.382
                                Layout.alignment: Qt.AlignVCenter
                            }
                            TextField {
                                id: passphrase
                                text: ""
                                echoMode: TextInput.Password
                                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase | Qt.ImhSensitiveData
                                Layout.alignment: Qt.AlignVCenter
                                Layout.fillWidth: true
                            }
                        }
                        RowLayout {
                            spacing: 10

                            Button {
                                text: qsTr("Connect")
                                onClicked: {
                                    connectView.visible = false
                                    NetworkSettingsManager.userAgent.setPassphrase(passphrase.text)
                                    if (networkSelection.currentIndex != -1) {
                                        NetworkSettingsManager.services.itemFromRow(networkSelection.currentIndex).connectService();
                                    }
                                }
                            }
                            Button {
                                text: qsTr("Cancel")
                                onClicked: {
                                    networkSelection.currentIndex = -1
                                    connectView.visible = false
                                }
                            }
                        }
                    }
                }
                Button {
                    id: manualConnect
                    visible: true
                    text: qsTr("Connect manually")
                    onClicked: {
                        visible = false
                        connectBySsidView.visible = true
                    }
                }
                GroupBox {
                    id: connectBySsidView
                    title: qsTr("Connect by an SSID")
                    visible: false
                    Layout.fillWidth: true
                    ColumnLayout {
                        width: parent.width

                        RowLayout {
                            spacing: 10
                            width: parent.width

                            Label {
                                text: qsTr("SSID:")
                                horizontalAlignment: Text.AlignRight
                                Layout.preferredWidth: root.width * 0.382
                                Layout.alignment: Qt.AlignVCenter
                            }
                            TextField {
                                id: ssid
                                text: ""
                                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase
                                Layout.alignment: Qt.AlignVCenter
                                Layout.fillWidth: true
                            }
                        }
                        RowLayout {
                            spacing: 10
                            width: parent.width
                            Label {
                                text: qsTr("Passphrase:")
                                horizontalAlignment: Text.AlignRight
                                Layout.preferredWidth: root.width * 0.382
                                Layout.alignment: Qt.AlignVCenter
                            }
                            TextField {
                                id: passphrase2
                                text: ""
                                echoMode: TextInput.Password
                                inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhPreferLowercase | Qt.ImhSensitiveData
                                Layout.alignment: Qt.AlignVCenter
                                Layout.fillWidth: true
                            }
                        }
                        RowLayout {
                            spacing: 10

                            Button {
                                text: qsTr("Connect")
                                onClicked: {
                                    NetworkSettingsManager.connectBySsid(ssid.text, passphrase2.text)
                                    connectBySsidView.visible = false
                                    manualConnect.visible = true
                                }
                            }
                            Button {
                                text: qsTr("Cancel")
                                onClicked: {
                                    connectBySsidView.visible = false
                                    manualConnect.visible = true
                                }
                            }
                        }
                    }
                }
                ColumnLayout {
                    spacing: parent.spacing
                    width: parent.width
                    visible: selectedInterface.state === NetworkSettingsState.Online ||
                             selectedInterface.state === NetworkSettingsState.Ready
                    Label {
                        text: qsTr("IP Address: ") + (NetworkSettingsManager.currentWifiConnection ? NetworkSettingsManager.currentWifiConnection.ipv4.address : "")
                    }
                    Button {
                        id: disconnect
                        text: qsTr("Disconnect")
                        onClicked: {
                            if (NetworkSettingsManager.currentWifiConnection) {
                                NetworkSettingsManager.currentWifiConnection.disconnectService();
                            }
                            networkSelection.currentIndex = -1;
                            root.connecting = false
                        }
                    }
                }
            }
            Connections {
                target: NetworkSettingsManager.userAgent
                onShowUserCredentialsInput : {
                    connectView.visible = true
                    root.connecting = false
                }
                onError: {
                    errorView.visible = true
                    connectView.visible = true
                    root.connecting = false
                    if (networkSelection.currentIndex != -1) {
                        NetworkSettingsManager.services.itemFromRow(networkSelection.currentIndex).removeService();
                    }
                }
            }
        }
    }
}
