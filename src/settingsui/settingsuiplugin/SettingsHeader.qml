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
import QtDeviceUtilities.NetworkSettings 1.0

Item {
    id: header
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.margins: pluginMain.margin
    height: pluginMain.margin * 4

    signal clicked()

    function refreshIP() {
        var networkCount = NetworkSettingsManager.services.sourceModel.rowCount();
        var ipAddress = "Not Found";
        for (var i = 0; i < networkCount; ++i) {
            var networkService = NetworkSettingsManager.services.itemFromRow(i);
            var tmp = NetworkSettingsManager.services.itemFromRow(i).ipv4.address;
            if (tmp !== "" && ipAddress === "Not Found")
                ipAddress = tmp;

            if (networkService.type === NetworkSettingsType.Wired &&
                    (networkService.status === NetworkSettingsState.Ready ||
                     networkService.status === NetworkSettingsState.Online)) {
                break;
            }
        }
        ipItem.text = qsTr("Wired IP: ") + ipAddress;
    }

    Image {
        id: backButton
        anchors.left: parent.left
        anchors.top: parent.top
        source: "../newIcons/back_icon.svg"
        height: parent.height * 0.5
        width: height / sourceSize.height * sourceSize.width
        MouseArea {
            anchors.fill: parent
            anchors.margins: -parent.height * 0.2
            onClicked: header.clicked()
        }
    }

    Text {
        id: settingsText
        anchors.left: backButton.right
        anchors.verticalCenter: backButton.verticalCenter
        font.pixelSize: parent.height * 0.45
        text: qsTr("Settings")
        color: "white"
        font.family: appFont
        font.styleName: "SemiBold"
        MouseArea {
            anchors.fill: parent
            anchors.margins: -parent.height * 0.2
            onClicked: header.clicked()
        }
    }

    Text {
        id: ipItem
        width: parent.width * 0.25
        height: parent.height/2
        anchors.top: settingsText.top
        anchors.right: parent.right
        anchors.rightMargin: pluginMain.margin
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: parent.height * 0.3
        color: "white"
        font.family: appFont
        font.styleName: "SemiBold"
        text: qsTr("Wired IP: ")

        MouseArea {
            anchors.fill: parent
            onClicked: header.refreshIP()
        }
    }

    Text {
        id: wifiIpItem
        width: parent.width * 0.25
        height: parent.height/2
        anchors.top: ipItem.bottom
        anchors.right: parent.right
        anchors.rightMargin: pluginMain.margin
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: parent.height * 0.3
        color: "white"
        font.family: appFont
        font.styleName: "SemiBold"
        text: qsTr((NetworkSettingsManager.currentWifiConnection ?
                   "Wireless IP: " +
                        NetworkSettingsManager.currentWifiConnection.ipv4.address : ""))
    }

    Component.onCompleted: {
        var networkCount = NetworkSettingsManager.services.sourceModel.rowCount();
        for (var i = 0; i < networkCount; ++i) {
            NetworkSettingsManager.services.itemFromRow(i).ipv4Changed.connect(refreshIP);
        }
        header.refreshIP()
    }
}
