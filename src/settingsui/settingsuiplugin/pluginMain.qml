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
import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import Qt.labs.settings 1.0
import QtQuick.XmlListModel 2.0
import QtDeviceUtilities.LocalDeviceSettings 1.0
import QtDeviceUtilities.NetworkSettings 1.0
import QtGraphicalEffects 1.0
import QtDemoLauncher.QtButtonImageProvider 1.0

Item {
    id: root
    anchors.fill: parent
    visible: true
    property var service
    property int margin: (root.width / 3 * 2) * 0.05
    property alias model: xmlModel.source
    property alias backgroundColor: background.color
    signal closed()

    Rectangle {
        id: background
        anchors.fill:parent
        color: "#09102b"
    }

    Item {
        id: header
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: parent.height * 0.125

        Image {
            id: backButton
            sourceSize.width: parent.width
            sourceSize.height: parent.height
            height: parent.height * 0.5
            width: height
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.margins: parent.height * .25
            fillMode: Image.PreserveAspectFit
            source: "../newIcons/back_icon.svg"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.closed()
                }
            }
        }

        Text {
            id: settingsText
            anchors.left: backButton.right
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: parent.height * 0.45
            text: qsTr("Settings")
            color: "white"
            font.family: appFont
            font.styleName: "SemiBold"
        }

        Item {
            id: ipItem
            width: parent.width * 0.25
            height: parent.height
            anchors.right: parent.right
            anchors.rightMargin: root.margin
            property alias ipAddressText: ipText.text
            property var lastService
            property var currentService: NetworkSettingsManager.services.itemFromRow(0)
            Text {
                id: ipText
                anchors.fill: parent
                horizontalAlignment: Text.AlignRight
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: parent.height * 0.25
                color: "white"
                text: qsTr("Device IP: ")
                font.family: appFont
                font.styleName: "SemiBold"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    parent.refreshIP()
                }
            }

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
                ipText.text = qsTr("Device IP: ") + ipAddress;
            }

            Component.onCompleted: {
                var networkCount = NetworkSettingsManager.services.sourceModel.rowCount();
                for (var i = 0; i < networkCount; ++i) {
                    NetworkSettingsManager.services.itemFromRow(i).ipv4Changed.connect(refreshIP);
                }
                refreshIP()
            }
        }
    }
    Item {
        id: listHolder
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width / 3

        XmlListModel {
            id: xmlModel
            source: "settingsview.xml"
            query: "/xml/settings/item"
            XmlRole { name: "title"; query: "title/string()"}
            XmlRole { name: "icon"; query: "icon/string()"}
            XmlRole { name: "view"; query: "view/string()"}
            XmlRole { name: "path"; query: "path/string()"}
        }

        ListView {
            id: settingsList
            anchors.fill: parent

            model: xmlModel
            delegate: Item {
                id: delegateItem
                width: settingsList.width
                height: settingsList.height * 0.08

                property bool isSelected: settingsList.currentIndex == index

                Image {
                    id: img
                    sourceSize.width: parent.width
                    sourceSize.height: parent.height
                    source: icon
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width * 0.075
                    height: Math.min(parent.width * 0.5, parent.height * 0.6)
                    width: height
                    fillMode: Image.PreserveAspectFit
                    visible: parent.isSelected ? false : true
                }
                ColorOverlay {
                    source: img
                    anchors.fill: img
                    visible: parent.isSelected ? true : false
                    color: "#41cd52"
                }
                Label {
                    text: title
                    height: parent.height * 0.75
                    anchors.right: parent.right
                    anchors.left: img.right
                    anchors.leftMargin: parent.width * 0.025
                    anchors.verticalCenter: parent.verticalCenter
                    fontSizeMode: Text.Fit
                    minimumPixelSize: 1
                    font.pixelSize: Math.min(parent.width * 0.25, parent.height)
                    color: parent.isSelected ? "#41cd52" : "white"
                    wrapMode: Text.WordWrap
                    verticalAlignment: Text.AlignVCenter
                    font.family: appFont
                    font.styleName: parent.isSelected ? "Bold" : "Regular"
                }
                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    onClicked: {
                        settingsList.currentIndex = index
                        settingsLoader.source = path + '/' + view + '.qml'
                    }
                }
                Component.onCompleted: {
                    if (xmlModel.count == 0) {
                        console.error("Empty xmlModel")
                        return
                    }
                    settingsLoader.source = xmlModel.get(0).path + '/' + xmlModel.get(0).view + '.qml'
                }
            }
        }
    }

    Rectangle {
        id: rightLine
        width: parent.width * 0.002
        height: parent.height * 0.8
        anchors.horizontalCenter: listHolder.right
        anchors.top: header.bottom
        color: "#9d9faa"
    }

    Item {
        id: loaderItem
        anchors.top: header.bottom
        anchors.left: listHolder.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: root.margin
        anchors.leftMargin: root.margin
        anchors.rightMargin: root.margin

        Loader {
            id: settingsLoader
            anchors.fill: parent
        }
    }

    Popup {
        id: shutdownPopup
        width: parent.width
        height: parent.height
        leftPadding: width * 0.3
        rightPadding: leftPadding
        topPadding: height * 0.275
        bottomPadding: topPadding

        property alias powerOffText: poweroffConfirmText.text
        property alias buttonText: shutdownConfirm.text

        background: Rectangle {
            color: "#09102b"
            opacity: 0.9
        }
        contentItem: Rectangle {
            color: "#09102b"
            border.color: "#9d9faa"
            border.width: 3
            Text {
                id: poweroffConfirmText
                width: parent.width * 0.75
                height: parent.height * 0.1
                anchors.top: parent.top
                anchors.topMargin: parent.height * 0.175
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                fontSizeMode: Text.Fit
                minimumPixelSize: 1
                font.pixelSize: height * 0.9
                color: "white"
                font.family: appFont
                font.styleName: "Light"
            }
            QtButton {
                id: shutdownConfirm
                width: parent.width * 0.5
                height: parent.height * 0.15
                anchors.top: poweroffConfirmText.bottom
                anchors.topMargin: parent.height * 0.15
                anchors.horizontalCenter: parent.horizontalCenter
                fillColor: "#41cd52"
                borderColor: "transparent"

                onClicked: {
                    if (shutdownConfirm.text === qsTr("SHUT DOWN")) {
                        console.log("Powering off!")
                        LocalDevice.powerOff()
                    }
                    else if (shutdownConfirm.text === qsTr("REBOOT")) {
                        console.log("Rebooting!")
                        LocalDevice.reboot()
                    }
                }
            }
            QtButton {
                id: shutdownCancel
                width: parent.width * 0.5
                height: parent.height * 0.15
                anchors.top: shutdownConfirm.bottom
                anchors.topMargin: parent.height * 0.1
                anchors.horizontalCenter: parent.horizontalCenter
                fillColor: "#9d9faa"
                borderColor: "transparent"
                text: qsTr("CANCEL")

                onClicked: {
                    shutdownPopup.close()
                }
            }
        }
    }
}
