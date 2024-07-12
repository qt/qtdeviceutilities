// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtQuick.Controls
import QtDeviceUtilities.NetworkSettings
import DeviceUtilities.SettingsUI
import DeviceUtilities.QtButtonImageProvider

Rectangle {
    id: passphrasePopup
    width: parent.width
    height: parent.height
    color: Globals.backgroundColor
    opacity: 0.9
    property string extraInfo: ""
    property bool showSsid: false

    property int margin: (width / 3 * 2) * 0.05
    property int spacing: margin * 0.5

    Rectangle {
        id: frame
        color: Globals.backgroundColor
        border.color: Globals.borderColor
        border.width: 3
        anchors.centerIn: parent
        width: passphraseColumn.width * 1.1
        height: passphraseColumn.height * 1.1

        Column {
            id: passphraseColumn
            anchors.centerIn: parent
            spacing: spacing

            Text {
                visible: showSsid
                font.pixelSize: passphrasePopup.height * Globals.subTitleFontSize
                font.family: Globals.appFont
                color: "white"
                text: qsTr("Enter SSID")
            }

            TextField {
                id: ssidField
                visible: showSsid
                width: passphrasePopup.width * 0.4
                height: passphrasePopup.height * 0.075
                color: "white"
                background: Rectangle{
                    color: "transparent"
                    border.color: ssidField.focus ? Globals.buttonGreenColor : Globals.buttonGrayColor
                    border.width: ssidField.focus ? width * 0.01 : 2
                }
            }

            Text {
                font.pixelSize: passphrasePopup.height * Globals.subTitleFontSize
                font.family: Globals.appFont
                color: "white"
                text: qsTr("Enter Passphrase")
            }

            Text {
                font.pixelSize: passphrasePopup.height * Globals.valueFontSize
                font.family: Globals.appFont
                color: "red"
                text: extraInfo
                visible: (extraInfo !== "")
            }

            TextField {
                id: passField
                width: passphrasePopup.width * 0.4
                height: passphrasePopup.height * 0.075
                color: "white"
                echoMode: TextInput.Password
                background: Rectangle{
                    color: "transparent"
                    border.color: passField.focus ? Globals.buttonGreenColor : Globals.buttonGrayColor
                    border.width: passField.focus ? width * 0.01 : 2
                }
            }

            Row {
                spacing: parent.width * 0.025
                QtButton{
                    id: setButton
                    text: qsTr("SET")
                    onClicked: {
                        if (showSsid) {
                            NetworkSettingsManager.connectBySsid(ssidField.text, passField.text)
                            showSsid = false
                        } else {
                            NetworkSettingsManager.userAgent.setPassphrase(passField.text)
                        }
                        passphrasePopup.visible = false;
                    }
                }
                QtButton {
                    id: cancelButton
                    text: qsTr("CANCEL")
                    borderColor: "transparent"
                    fillColor: Globals.buttonGrayColor
                    onClicked: {
                        if (!showSsid) {
                            NetworkSettingsManager.userAgent.cancelInput()
                        }
                        showSsid = false
                        passphrasePopup.visible = false;
                    }
                }
            }
        }
    }
}
