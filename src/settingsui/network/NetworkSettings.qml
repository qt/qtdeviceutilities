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
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtDeviceUtilities.NetworkSettings 1.0
import QtDeviceUtilities.QtButtonImageProvider 1.0
import QtDeviceUtilities.LocalDeviceSettings 1.0
import "../common"
import "../timedate"

Item {
    id: networkSettingsRoot
    property string title: qsTr("Network Settings")
    property bool usbEthernetSettingVisibility: ( viewSettings.usbEthernetSettingVisible && NetworkSettingsManager.hasUsbEthernetProtocolConfiguration() ) ? true : false
    anchors.fill: parent
    Text {
        id: usbEthernetTitleText
        visible: usbEthernetSettingVisibility
        text: qsTr("USB Ethernet")
        font.pixelSize: pluginMain.subTitleFontSize
        font.family: appFont
        font.styleName: "SemiBold"
        color: "white"
        anchors.left: parent.left
    }
    TextArea {
        id: ipAddressTextArea
        visible: usbEthernetSettingVisibility
        text: qsTr("IP Address:") + NetworkSettingsManager.usbEthernetIpAddress
        color: viewSettings.buttonGreenColor
        font.family: appFont
        font.styleName: "SemiBold"
        font.pixelSize: pluginMain.subTitleFontSize - 2
        opacity: 1.0
        readOnly: true
        anchors.left: usbEthernetTitleText.left
        anchors.verticalCenter: usbEthernetCustomComboBox.verticalCenter
        anchors.leftMargin: 15
    }
    CustomComboBox {
        id: usbEthernetCustomComboBox
        visible: usbEthernetSettingVisibility
        width: root.width * 0.15
        height: pluginMain.buttonHeight
        anchors.top: usbEthernetTitleText.bottom
        anchors.left: ipAddressTextArea.right
        anchors.right: setUsbEthernetButton.left
        anchors.rightMargin: 15
        anchors.leftMargin: 15
        model: ["RNDIS", "CDCECM" ]
        currentIndex: 0
        delegate: ItemDelegate {
            id: usbEthernetDelegate
            contentItem: Text {
                anchors.left: usbEthernetDelegate.left
                anchors.leftMargin: pluginMain.margin
                text: modelData
                color: usbEthernetCustomComboBox.currentIndex == index ? viewSettings.buttonGreenColor : "white"
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: pluginMain.valueFontSize
                font.family: appFont
                font.styleName: "Regular"
            }
        }
        Component.onCompleted: {
            usbEthernetCustomComboBox.currentIndex = "RNDIS" === NetworkSettingsManager.usbEthernetProtocol ? 0 : 1
        }
    }
    QtButton {
        id: setUsbEthernetButton
        visible: usbEthernetSettingVisibility
        height: pluginMain.buttonHeight
        text: qsTr("SAVE & REBOOT")
        anchors.right: parent.right
        anchors.verticalCenter: usbEthernetCustomComboBox.verticalCenter
        onClicked: {
            showRebootAcceptPopup();
        }
    }

    function showRebootAcceptPopup() {
        messageDialog.visible = true
    }

    Dialog {
        function rebootAccepted() {
            NetworkSettingsManager.setUsbVirtualEthernetLinkProtocol(usbEthernetCustomComboBox.currentText)
            LocalDevice.reboot()
        }
        id: messageDialog
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        opacity: 0.9
        background: Rectangle{
            id: messageDialogMainRectangle
            width: parent.width
            height: parent.height
            color: viewSettings.backgroundColor
        }
        Rectangle {
            id: messageDialogPopupRectangle
            color: viewSettings.backgroundColor
            border.color: viewSettings.borderColor
            border.width: 3
            anchors.centerIn: parent
            width: parent.width * 0.75
            height: parent.height * 0.75
            Column {
                anchors.centerIn: parent
                spacing: viewSettings.pageMargin
                Text {
                    id: shutDownConfirmText
                    width: messageDialogPopupRectangle.width * 0.75
                    height: messageDialogPopupRectangle.height * 0.25
                    horizontalAlignment: Text.AlignHCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    fontSizeMode: Text.Fit
                    minimumPixelSize: 1
                    font.pixelSize: messageDialogPopupRectangle.width * 0.3
                    color: "white"
                    font.family: viewSettings.appFont
                    font.styleName: "SemiBold"
                    text: "Save and reboot the system?"
                }
                QtButton {
                    id: saveRebootConfirm
                    height: pluginMain.buttonHeight
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: "OK"
                    onClicked: messageDialog.rebootAccepted();
                }
                QtButton {
                    id: saveRebootCancel
                    height: pluginMain.buttonHeight
                    anchors.horizontalCenter: parent.horizontalCenter
                    borderColor: "transparent"
                    fillColor: viewSettings.buttonGrayColor
                    text: qsTr("CANCEL")
                    onClicked: {
                        messageDialog.visible = false
                    }
                }
            }
        }
    }
    Text {
        id: wlanText
        text: qsTr("WLAN")
        font.pixelSize: pluginMain.subTitleFontSize
        font.family: appFont
        font.styleName: "SemiBold"
        color: "white"
        anchors.top: usbEthernetSettingVisibility ? usbEthernetCustomComboBox.bottom : networkSettingsRoot.top
        anchors.left: parent.left
    }
    CustomSwitch {
        id: wifiSwitch
        anchors.top: wlanText.bottom
        anchors.left: wlanText.left
        height: pluginMain.buttonHeight
        indicatorWidth: pluginMain.buttonWidth
        indicatorHeight: pluginMain.buttonHeight
        property bool wiFiAvailable: NetworkSettingsManager.interface(NetworkSettingsType.Wifi, 0) !== null
        enabled: wiFiAvailable && !wifiSwitchTimer.running
        onCheckedChanged: {
            // Power on/off all WiFi interfaces
            for (var i = 0; NetworkSettingsManager.interface(NetworkSettingsType.Wifi, i) !== null; i++) {
                NetworkSettingsManager.interface(NetworkSettingsType.Wifi, i).powered = checked
                wifiSwitchTimer.start()
            }
        }
        Component.onCompleted: {
            // If any of the WiFi interfaces is powered on, switch is checked
            var checkedStatus = false;
            for (var i = 0; NetworkSettingsManager.interface(NetworkSettingsType.Wifi, i) !== null; i++) {
                if (NetworkSettingsManager.interface(NetworkSettingsType.Wifi, i).powered) {
                    checkedStatus = true;
                    break;
                }
            }
            checked = checkedStatus;
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
        anchors.rightMargin: 15
        enabled: wifiSwitch.checked
        fillColor: enabled ? viewSettings.buttonGreenColor : viewSettings.buttonGrayColor
        borderColor: "transparent"
        height: pluginMain.buttonHeight
        text: qsTr("MANUAL CONNECT")
        onClicked: {
            networkList.connectBySsid()
        }
    }
    QtButton {
        id: manualDisconnect
        anchors.top: wlanText.bottom
        anchors.right: parent.right
        enabled: NetworkSettingsManager.currentWifiConnection
        fillColor: enabled ? viewSettings.buttonGreenColor : viewSettings.buttonGrayColor
        borderColor: "transparent"
        height: pluginMain.buttonHeight
        text: qsTr("DISCONNECT")
        onClicked: {
            if (NetworkSettingsManager.currentWifiConnection) {
                NetworkSettingsManager.currentWifiConnection.disconnectService();
            }
        }
    }
    Text {
        id: networkListTextItem
        text: qsTr("Network list")
        font.pixelSize: pluginMain.subTitleFontSize
        font.family: appFont
        font.styleName: "SemiBold"
        color: "white"
        anchors.top: wifiSwitch.bottom
    }
    NetworkListView {
        id: networkList
        anchors.top: networkListTextItem.bottom
        anchors.left: networkListTextItem.left
        width: networkSettingsRoot.width
        height: networkSettingsRoot.height
    }
}

