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
import QtQuick.Controls.Styles.Flat 1.0 as Flat
import "../common"
import com.theqtcompany.settings.network 1.0

Item {
    id: root
    anchors.fill: parent
    anchors.margins: Math.round(20 * Flat.FlatStyle.scaleFactor)

    Component.onCompleted: {
        NetworkSettingsManager.services.type = NetworkSettingsType.Wifi;
    }
    GroupBox {
        id: content
        title: qsTr("Wireless Settings")
        anchors.fill: parent
        Layout.fillWidth: true
        flat: true

        ColumnLayout {
            spacing: Math.round(20 * Flat.FlatStyle.scaleFactor)
            width: parent.width
            Row {
                id: enableSwitch
                width: parent.width
                spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                TextLabel {
                    width:  root.width*0.382
                    horizontalAlignment: Text.AlignRight
                    text: selectedInterface.powered ? qsTr("Disable Wifi") : qsTr("Enable Wifi")
                }
                Switch {
                    checked: selectedInterface.powered
                    onCheckedChanged: selectedInterface.powered = checked
                }
            }

            Row {
                spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                width: parent.width

                TextLabel {
                    id: labelText
                    text: qsTr("Selected network ")
                    width: content.width*0.382
                    horizontalAlignment: Text.AlignRight
                }

                CustomCombobox {
                    id: networkSelection
                    model: NetworkSettingsManager.services
                    visible: selectedInterface.powered
                    width: Math.round(200 * Flat.FlatStyle.scaleFactor)
                    textRole: "name"
                    onSelectedIndexChanged : if (selectedIndex >= 0) model.itemFromRow(selectedIndex).connectService();

                    delegate: WifiSelectorDelegate {
                        id: delegate
                        onConnectChanged: if (connect) networkSelection.setSelectIndexToVal(modelData.name, "name");
                    }
                }
            }

            GroupBox {
                id: connectView
                title: qsTr("Enter a password")
                flat: false
                visible: false
                ColumnLayout {
                    Row {
                        id: errorView
                        property alias text: text.text
                        visible: text.text !== ""

                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        Image {
                            source: "../icons/Alert_yellow_1x.png"
                        }
                        Text {
                            id: text
                            color: "#face20"
                            text: ""
                        }
                    }
                    Row {
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        visible: false
                        TextLabel {
                            text: qsTr("User name")
                            width: root.width*0.382
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            text: ""
                            inputMethodHints: Qt.ImhNoPredictiveText
                        }
                    }
                    Row {
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        TextLabel {
                            text: qsTr("Password")
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            id: password
                            text: ""
                            echoMode: TextInput.Password
                            inputMethodHints: Qt.ImhNoPredictiveText
                        }
                    }
                    Row {
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        Button {
                            text: qsTr("Connect")
                            onClicked: {
                                connectView.visible = false
                                NetworkSettingsUserAgent.setUserCredentials("", password.text)
                            }
                        }
                        Button {
                            text: qsTr("Cancel")
                            onClicked:connectView.visible = false
                        }
                    }
                }
            }

            Button {
                id: disconnect
                text: qsTr("Disconnect")
                visible: selectedInterface.state === NetworkSettingsState.Online ||
                         selectedInterface.state === NetworkSettingsState.Ready
                onClicked: {
                    NetworkSettingsManager.services.itemFromRow(networkSelection.selectedIndex).disconnectService();
                    networkSelection.selectedIndex = -1;
                }
            }
        }

        Connections {
            target: NetworkSettingsUserAgent
            onShowUserCredentialsInput : {
                connectView.visible = true
            }
            onError: {
                errorView.visible = true
                connectView.visible = true
            }
        }
    }
}
