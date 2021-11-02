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

Item {
    id: header
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.margins: margin
    height: margin * 4

    property int margin: Globals.margin(header.width)
    property int spacing: margin * 0.5

    signal clicked()

    Image {
        id: networkButton
        anchors.left: header.left
        anchors.top: header.top
        source: "icons/back.svg"
        height: header.height * 0.5
        width: height / sourceSize.height * sourceSize.width
        MouseArea {
            anchors.fill: parent
            anchors.margins: -parent.height * 0.2
            onClicked: parent.clicked()
        }
        visible: false
    }

    Text {
        id: settingsText
        anchors.left: networkButton.right
        anchors.verticalCenter: networkButton.verticalCenter
        font.pixelSize: header.height * 0.45
        text: qsTr("Settings")
        color: "white"
        font.family: Globals.appFont
        font.styleName: "SemiBold"
        MouseArea {
            anchors.fill: parent
            anchors.margins: -parent.height * 0.2
            onClicked: parent.clicked()
        }
    }

    Text {
        id: ipItem
        width: header.width * 0.25
        height: header.height/2
        anchors.top: settingsText.top
        anchors.right: header.right
        anchors.rightMargin: margin
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: header.height * 0.3
        color: "white"
        font.family: Globals.appFont
        font.styleName: "SemiBold"
        text: NetworkSettingsManager.currentWiredConnection ?
                   qsTr("Wired IP: ") +
                        NetworkSettingsManager.currentWiredConnection.ipv4.address : ""
    }

    Text {
        id: wifiIpItem
        width: header.width * 0.25
        height: header.height/2
        anchors.top: ipItem.bottom
        anchors.right: parent.right
        anchors.rightMargin: margin
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: header.height * 0.3
        color: "white"
        font.family: Globals.appFont
        font.styleName: "SemiBold"
        text: NetworkSettingsManager.currentWifiConnection ?
                   qsTr("Wireless IP: ") +
                        NetworkSettingsManager.currentWifiConnection.ipv4.address : ""
    }
}
