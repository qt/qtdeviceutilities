// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtDeviceUtilities.NetworkSettings

Item {
    id: root
    anchors.fill: parent
    Component.onCompleted: NetworkSettingsManager.services.type = NetworkSettingsType.Wifi;
    property bool connecting: false
    property var selectedInterface: undefined

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
                        onActivated: function(index) {
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
                                source: "icons/Alert_yellow_1x.png"
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
                ColumnLayout {
                    spacing: parent.spacing
                    width: parent.width
                    visible: selectedInterface.state === NetworkSettingsState.Online ||
                             selectedInterface.state === NetworkSettingsState.Ready
                    Label {
                        text: qsTr("IP Address: ") + NetworkSettingsManager.services.itemFromRow(networkSelection.currentIndex).ipv4.address
                    }
                    Button {
                        id: disconnect
                        text: qsTr("Disconnect")
                        onClicked: {
                            NetworkSettingsManager.services.itemFromRow(networkSelection.currentIndex).disconnectService();
                            networkSelection.currentIndex = -1;
                            root.connecting = false
                        }
                    }
                }
            }
            Connections {
                target: NetworkSettingsManager.userAgent
                function onShowUserCredentialsInput() {
                    connectView.visible = true
                    root.connecting = false
                }
                function onError() {
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
