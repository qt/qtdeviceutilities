/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://www.qt.io
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://www.qt.io
**
****************************************************************************/
import QtQuick 2.3
import QtQuick.Controls 1.2
import B2Qt.Wifi 1.0

Item {
    anchors.fill: parent

    Binding {
        target: WifiManager
        property: "scanning"
        value: networkView.visible
    }

    Button {
        id: wifiOnOffButton
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.right: parent.right
        onClicked: {
            if (WifiManager.backendState === WifiManager.Running) {
                if (networkView.visible)
                    networkView.visible = false
                WifiManager.stop()
            } else if (WifiManager.backendState === WifiManager.NotRunning) {
                WifiManager.start()
            }
        }

        Component.onCompleted: updateButtonText(WifiManager.backendState)
        Connections {
            target: WifiManager
            onBackendStateChanged: wifiOnOffButton.updateButtonText(backendState)
        }

        function updateButtonText(backendState)
        {
            if (backendState === WifiManager.Initializing)
                wifiOnOffButton.text = "Initializing..."
            if (backendState === WifiManager.Terminating)
                wifiOnOffButton.text = "Terminating..."
            if (backendState === WifiManager.NotRunning)
                wifiOnOffButton.text = "Switch On"
            if (backendState === WifiManager.Running)
                wifiOnOffButton.text = "Switch Off"
        }
    }

    Button {
        id: listNetworksButton
        anchors.top: wifiOnOffButton.bottom
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.right: parent.right
        visible: WifiManager.backendState === WifiManager.Running
        text: networkView.visible ? "Hide wifi networks"
                                  : "List available wifi networks"
        onClicked: networkView.visible = !networkView.visible
    }
    //! [0]
    ListView {
        id: networkView
        model: WifiManager.networks
        delegate: listDelegate
        implicitHeight: 800
        anchors.top: listNetworksButton.bottom
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        visible: false
        clip: true

        property string networkStateText: ""
        property QtObject expandedNetworkBox: null
        property bool hasExpandedNetworkBox: expandedNetworkBox !== null

        function setNetworkStateText(networkState) {
            if (networkState === WifiManager.ObtainingIPAddress)
                networkView.networkStateText = " (obtaining ip..)"
            else if (networkState === WifiManager.DhcpRequestFailed)
                networkView.networkStateText = " (dhcp request failed)"
            else if (networkState === WifiManager.Connected)
                networkView.networkStateText = " (connected)"
            else if (networkState === WifiManager.Authenticating)
                networkView.networkStateText = " (authenticating..)"
            else if (networkState === WifiManager.HandshakeFailed)
                networkView.networkStateText = " (wrong password)"
            else if (networkState === WifiManager.Disconnected)
                networkView.networkStateText = ""
        }

        Connections {
            target: WifiManager
            onNetworkStateChanged: networkView.setNetworkStateText(networkState)
        }

        Component.onCompleted: {
            if (WifiManager.backendState == WifiManager.Running)
                networkView.visible = true
        }
    }
    //! [0]
    //! [2]
    WifiConfiguration { id: config }
    //! [2]
    Component {
        id: listDelegate
        Rectangle {
            id: networkBox
            property bool expanded: false
            property bool isCurrentNetwork: WifiManager.currentSSID === ssid
            property bool connected: isCurrentNetwork && WifiManager.networkState === WifiManager.Connected
            property int notExpandedHeight: ssidLabel.height + bssidLabel.height + 20
            property int expandedHeight: notExpandedHeight + passwordInput.height + connectionButton.height + 54
            property int connectedExpandedHeight: notExpandedHeight + connectionButton.height + 30
            height: expanded ? (connected ? connectedExpandedHeight : expandedHeight) : notExpandedHeight
            width: parent.width
            clip: true
            border.color: "black"
            border.width: 1

            Component.onDestruction: if (expanded) networkView.expandedNetworkBox = null
            onHeightChanged: if (expanded) networkView.positionViewAtIndex(index, ListView.Contain)

            Behavior on height { NumberAnimation { duration: 500; easing.type: Easing.InOutCubic } }
            //! [1]
            Text {
                id: ssidLabel
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 5
                anchors.leftMargin: 10
                font.pixelSize: 26
                font.bold: true
                text: isCurrentNetwork ? ssid + networkView.networkStateText : ssid
                Component.onCompleted: networkView.setNetworkStateText(WifiManager.networkState)
            }

            Text {
                id: bssidLabel
                anchors.top: ssidLabel.bottom
                anchors.left: parent.left
                anchors.margins: 5
                anchors.leftMargin: 30
                text: bssid
                font.pixelSize: ssidLabel.font.pixelSize * 0.8
            }

            Text {
                id: flagsLabel
                anchors.top: bssidLabel.top
                anchors.left: bssidLabel.right
                anchors.leftMargin: 35
                text: (supportsWPA2 ? "WPA2 " : "")
                     + (supportsWPA ? "WPA " : "")
                     + (supportsWEP ? "WEP " : "")
                     + (supportsWPS ? "WPS " : "");
                font.pixelSize: ssidLabel.font.pixelSize * 0.8
                font.italic: true
            }

            ProgressBar {
                id: signalStrengthBar
                height: 20
                width: networkBox.width * 0.5
                anchors.margins: 10
                anchors.right: parent.right
                anchors.top: parent.top
                minimumValue: 0
                maximumValue: 100
                value : signalStrength
            }
            //! [1]
            MouseArea {
                anchors.fill: parent
                onClicked: handleNetworkBoxExpanding()
            }

            function handleNetworkBoxExpanding()
            {
                expanded = !expanded
                if (expanded) {
                    if (networkView.hasExpandedNetworkBox)
                        networkView.expandedNetworkBox.expanded = false
                    networkView.expandedNetworkBox = networkBox
                } else {
                    networkView.expandedNetworkBox = null
                }
            }

            TextField {
                id: passwordInput
                anchors.top: flagsLabel.bottom
                anchors.topMargin: 15
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width * 0.36
                height: connectionButton.height * 1.1
                placeholderText: "Enter Password"
                visible: !connected
                font.pixelSize: 16
                echoMode: TextInput.Password
                inputMethodHints: Qt.ImhNoPredictiveText
            }

            Button {
                id: connectionButton
                y: connected ? passwordInput.y
                             : passwordInput.y + passwordInput.height + 10
                width: passwordInput.width
                anchors.horizontalCenter: parent.horizontalCenter
                text: connected ? "Disconnect" : "Connect"
                //! [3]
                onClicked: {
                    if (connected) {
                        WifiManager.disconnect()
                    } else {
                        config.ssid = ssid;
                        config.passphrase = passwordInput.text
                        if (!WifiManager.connect(config))
                            print("failed to connect: " + WifiManager.lastError)
                    }
                }
                //! [3]
            }
        }
    }
}
