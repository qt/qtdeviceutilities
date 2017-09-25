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
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.0
import QtDeviceUtilities.QtButtonImageProvider 1.0
import QtDeviceUtilities.NetworkSettings 1.0

Item {
    id: root
    Row {
        id: backRow
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width * 0.2
        height: parent.height * 0.035
        Image {
            id: passBackIcon
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height
            width: height
            fillMode: Image.PreserveAspectFit
            source: "../newIcons/back_icon.svg"

            ColorOverlay {
                source: passBackIcon
                anchors.fill: passBackIcon
                color: "#41cd52"
                visible: true
            }
            MouseArea {
                anchors.fill: parent
                onClicked: settingsLoader.source = "qrc:/network/NetworkSettings.qml"
            }
        }
        Text {
            id: backText
            anchors.top: parent.top
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            fontSizeMode: Text.Fit
            minimumPixelSize: 1
            font.pixelSize: height
            color: "#41cd52"
            text: "Back"
            MouseArea {
                anchors.fill: parent
                onClicked: settingsLoader.source = "qrc:/network/NetworkSettings.qml"
            }
        }
    }

    Text {
        id: enterPassphraseText
        anchors.top: backRow.bottom
        anchors.topMargin: parent.height * 0.05
        anchors.left: parent.left
        fontSizeMode: Text.Fit
        minimumPixelSize: 1
        font.pixelSize: parent.height * 0.045
        color: "white"
        text: qsTr("Enter Passphrase")
    }
    Rectangle {
        id: btmLine
        anchors.top: enterPassphraseText.bottom
        anchors.topMargin: parent.height * 0.025
        anchors.left: enterPassphraseText.left
        width: parent.width * 0.275
        height: parent.height * 0.005
    }

    TextField {
        id: passField
        width: root.width * 0.4
        height: parent.height * 0.075
        anchors.top: btmLine.bottom
        anchors.topMargin: parent.height * 0.05
        color: "white"
        echoMode: TextInput.Password
        background: Rectangle {
            color: "transparent"
            border.color: passField.focus ? "#41cd52" : "#9d9faa"
            border.width: passField.focus ? width * 0.01 : 2
        }
    }
    Row {
        height: passField.height
        anchors.top: passField.bottom
        anchors.topMargin: parent.height * 0.025
        spacing: parent.width * 0.025
        QtButton {
            id: setButton
            height: parent.height
            text: qsTr("SET")
            borderColor: "transparent"
            fillColor: "#41cd52"
            onClicked: {
                passField.focus = false
                NetworkSettingsManager.userAgent.setPassphrase(passField.text)
            }
        }
        QtButton {
            id: cancelButton
            height: parent.height
            text: qsTr("CANCEL")
            borderColor: "transparent"
            fillColor: "#9d9faa"
            onClicked: {
                passField.focus = false
                passField.clear()
            }
        }
    }
}
