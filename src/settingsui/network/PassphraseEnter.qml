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
import QtQuick
import QtQuick.Controls
import QtDeviceUtilities.QtButtonImageProvider
import QtDeviceUtilities.NetworkSettings

Rectangle {
    id: passphrasePopup
    width: root.width
    height: root.height
    color: viewSettings.backgroundColor
    opacity: 0.9
    property string extraInfo: ""
    property bool showSsid: false

    Rectangle {
        id: frame
        color: viewSettings.backgroundColor
        border.color: viewSettings.borderColor
        border.width: 3
        anchors.centerIn: parent
        width: passphraseColumn.width * 1.1
        height: passphraseColumn.height * 1.1

        Column {
            id: passphraseColumn
            anchors.centerIn: parent
            spacing: viewSettings.spacing

            Text {
                visible: showSsid
                font.pixelSize: pluginMain.subTitleFontSize
                font.family: appFont
                color: "white"
                text: qsTr("Enter SSID")
            }

            TextField {
                id: ssidField
                visible: showSsid
                width: root.width * 0.4
                height: root.height * 0.075
                color: "white"
                background: Rectangle{
                    color: "transparent"
                    border.color: ssidField.focus ? viewSettings.buttonGreenColor : viewSettings.buttonGrayColor
                    border.width: ssidField.focus ? width * 0.01 : 2
                }
            }

            Text {
                font.pixelSize: pluginMain.subTitleFontSize
                font.family: appFont
                color: "white"
                text: qsTr("Enter Passphrase")
            }

            Text {
                font.pixelSize: pluginMain.valueFontSize
                font.family: appFont
                color: "red"
                text: extraInfo
                visible: (extraInfo !== "")
            }

            TextField {
                id: passField
                width: root.width * 0.4
                height: root.height * 0.075
                color: "white"
                echoMode: TextInput.Password
                background: Rectangle{
                    color: "transparent"
                    border.color: passField.focus ? viewSettings.buttonGreenColor : viewSettings.buttonGrayColor
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
                    fillColor: viewSettings.buttonGrayColor
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
