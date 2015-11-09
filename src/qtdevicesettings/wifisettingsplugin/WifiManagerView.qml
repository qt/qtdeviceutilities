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
import com.theqtcompany.settings.common 1.0
import com.theqtcompany.settings.wifi 1.0
import B2Qt.Wifi 1.0

Item {
    id: root
    Component.onCompleted: {
        if (WifiManager.backendState === WifiManager.NotRunning) {
            WifiManager.start();
        }
        else if (WifiManager.backendState == WifiManager.Running) {
            WifiManager.scanning = true;
        }
    }

    WifiConfiguration {
        id: config
         property bool connected: WifiManager.networkState === WifiManager.Connected
    }

    ColumnLayout {
        id: content
        anchors.fill: parent

        Row {
            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)

            TextLabel {
                text: qsTr("Selected network ")
                width: root.width*0.382
                horizontalAlignment: Text.AlignRight
            }

            CustomCombobox {
                id: networkSelection
                model: WifiManager.networks
                width: Math.round(200 * Flat.FlatStyle.scaleFactor)
                textRole: "ssid"
                Component.onCompleted: {
                    setSelectIndexToVal(WifiManager.currentSSID, "ssid");
                }
                onSelectedIndexChanged : {
                    var ssid = networkSelection.textValue;
                    config.ssid = ssid;
                    if (WifiManager.currentSSID !== ssid || !config.connected) {
                        connectView.visible = true;
                    }
                }
                delegate: WifiSelectorDelegate { }
            }

            Image {
                id: warning
                anchors.verticalCenter: parent.verticalCenter
                source: "../icons/Alert_yellow_1x.png"
                visible: WifiManager.state === WifiManager.HandshakeFailed ||
                         WifiManager.state === WifiManager.DhcpRequestFailed
            }
        }

        Button {
            id: disconnect
            text: qsTr("Disconnect")
            visible: config.connected
            onClicked: {
                WifiManager.disconnect();
                networkSelection.currentIndex = -1;
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
                            config.passphrase = password.text
                            if (!WifiManager.connect(config)) {
                                print("failed to connect: " + WifiManager.lastError)
                                errorView.text = qsTr("Invalid password");
                            }
                            else {
                                connectView.visible = false
                            }
                        }
                    }
                    Button {
                        text: qsTr("Cancel")
                        onClicked:connectView.visible = false
                    }
                }
            }
        }
    }
}
