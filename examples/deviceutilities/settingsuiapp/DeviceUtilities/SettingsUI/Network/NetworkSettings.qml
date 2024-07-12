// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtDeviceUtilities.NetworkSettings
import DeviceUtilities.QtButtonImageProvider
import DeviceUtilities.SettingsUI

Item {
    id: networkSettingsRoot
    anchors.fill: parent

    Connections {
        target: NetworkSettingsManager
        function onInterfacesChanged() {
            if (NetworkSettingsManager.interface(NetworkSettingsType.Wifi, 0) !== null) {
                wifiSwitch.visible = true
                wifiSwitch.checked = Qt.binding(function() { return NetworkSettingsManager.interface(NetworkSettingsType.Wifi, 0).powered })
            } else {
                wifiSwitch.visible = false
            }
        }
    }

    Text {
        id: wlanText
        visible: wifiSwitch.visible
        text: qsTr("WiFi")
        font.pixelSize: networkSettingsRoot.height * Globals.subTitleFontSize
        font.family: Globals.appFont
        font.styleName: "SemiBold"
        color: "white"
        anchors.top: networkSettingsRoot.top
        anchors.left: networkSettingsRoot.left
    }

    CustomSwitch {
        id: wifiSwitch
        anchors.top: wlanText.bottom
        anchors.left: wlanText.left
        height: networkSettingsRoot.height * Globals.buttonHeight
        indicatorWidth: networkSettingsRoot.height * Globals.buttonWidth
        indicatorHeight: networkSettingsRoot.height * Globals.buttonHeight
        checkable: visible && !wifiSwitchTimer.running

        onCheckedChanged: {
            // Power on/off all WiFi interfaces
            for (var i = 0; NetworkSettingsManager.interface(NetworkSettingsType.Wifi, i) !== null; i++) {
                NetworkSettingsManager.interface(NetworkSettingsType.Wifi, i).powered = checked
                wifiSwitchTimer.start()
            }
        }

        // At least 1s between switching on/off
        Timer {
            id: wifiSwitchTimer
            interval: 1000
            running: false
        }
    }
    QtButton {
        id: manualConnect
        anchors.top: wlanText.bottom
        anchors.left: wifiSwitch.right
        anchors.right: manualDisconnect.left
        anchors.rightMargin: 10
        visible: wifiSwitch.visible
        enabled: wifiSwitch.checked
        fillColor: enabled ? Globals.buttonGreenColor : Globals.buttonGrayColor
        borderColor: "transparent"
        height: networkSettingsRoot.height * Globals.buttonHeight
        text: qsTr("MANUAL CONNECT")
        onClicked: {
            networkList.connectBySsid()
        }
    }

    QtButton {
        id: manualDisconnect
        anchors.top: wlanText.bottom
        anchors.right: networkSettingsRoot.right
        visible: wifiSwitch.visible
        enabled: NetworkSettingsManager.currentWifiConnection
        fillColor: enabled ? Globals.buttonGreenColor : Globals.buttonGrayColor
        borderColor: "transparent"
        height: networkSettingsRoot.height * Globals.buttonHeight
        text: qsTr("DISCONNECT")
        onClicked: {
            if (NetworkSettingsManager.currentWifiConnection) {
                NetworkSettingsManager.currentWifiConnection.disconnectService();
            }
        }
    }

    Text {
        id: networkListTextItem
        text: qsTr("Available networks:")
        font.pixelSize: networkSettingsRoot.height * Globals.subTitleFontSize
        font.family: Globals.appFont
        font.styleName: "SemiBold"
        color: "white"
        anchors.top: (wifiSwitch.visible === true) ? wifiSwitch.bottom : networkSettingsRoot.top
        anchors.topMargin: 10
    }

    NetworkListView {
        id: networkList
        anchors.top: networkListTextItem.bottom
        anchors.left: networkListTextItem.left
        width: networkSettingsRoot.width
        height: networkSettingsRoot.height
    }
}

