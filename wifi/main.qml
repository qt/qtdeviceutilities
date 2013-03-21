import QtQuick 2.0

import QtWifi 0.1

Rectangle
{
    id: root

    color: Qt.rgba(Math.random(), Math.random(), Math.random(), 1);

    QWifiManager {
        id: wifiManager;

        Component.onCompleted: start();

        onReadyChanged: {
            print("QML: QWifiManager is now connected...");
        }

        onOnlineChanged: print(online ? "QML: WifiManager is online" : "QML: WifiManager is not online...");

        scanning: ready && connectedSSID == "";
    }

    Component {
        id: listDelegate

        Rectangle {
            id: delegateBackground
            property bool expanded: false
            height: expanded ? 300 : 70
            clip: true // ### fixme

            Behavior on height { NumberAnimation { duration: 500; easing.type: Easing.InOutCubic } }

            width: parent.width

            gradient: Gradient {
                GradientStop { position: 0; color: "white" }
                GradientStop { position: 67 / delegateBackground.height; color: "lightgray" }
                GradientStop { position: 1; color: "gray" }
            }

            Text {
                id: ssidLabel
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 10
                font.pixelSize: 24
                font.bold: true
                text: network.ssid + (wifiManager.connectedSSID == network.ssid ? " (connected)" : "");
            }

            Text {
                id: bssidLabel
                anchors.top: ssidLabel.bottom
                anchors.left: parent.left
                anchors.margins: 5
                anchors.leftMargin: 40
                text: network.bssid
                color: "gray"
                font.pixelSize: ssidLabel.font.pixelSize * 0.5
            }

            Text {
                id: flagsLabel
                x: 200
                anchors.top: bssidLabel.top
                text: (network.supportsWPA2 ? "WPA2 " : "")
                      + (network.supportsWPA ? "WPA " : "")
                      + (network.supportsWEP ? "WEP " : "")
                      + (network.supportsWPS ? "WPS " : "");
                color: "gray"
                font.pixelSize: ssidLabel.font.pixelSize * 0.5
                font.italic: true
            }

            Rectangle {
                width: Math.max(100 + network.signalStrength, 0) / 100 * parent.width;
                height: 20
                radius: 10
                antialiasing: true
                anchors.margins: 20
                anchors.right: parent.right
                anchors.top: parent.top
                color: "white"
                border.color: "lightgray"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    parent.expanded = !expanded
                }
            }

            Rectangle {
                id: passwordInputBackground
                anchors.fill: passwordInput
                anchors.margins: -5
                color: "white"
                radius: 5
                border.color: "gray"
            }

            TextInput {
                id: passwordInput
                y: 100
                width: 300
                height: 50
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 14
            }

            Rectangle {
                id: connectButton
                anchors.top: passwordInput.bottom
                anchors.margins: 20
                anchors.horizontalCenter: parent.horizontalCenter
                width: passwordInput.width
                height: passwordInputBackground.height

                gradient: Gradient {
                    GradientStop { position: 0; color: "white" }
                    GradientStop { position: 1; color: buttonMouse.pressed ? "steelblue" : "lightsteelblue" }
                }

                border.color: "gray"

                Text {
                    anchors.centerIn: parent
                    font.pixelSize: 24
                    text: "Connect"
                }
                MouseArea {
                    id: buttonMouse
                    anchors.fill: parent
                    onClicked: wifiManager.connect(network, passwordInput.text);
                }
            }

        }
    }

    ListView {
        anchors.fill: root
        model: wifiManager.networks
        delegate: listDelegate;
    }

    Image {
        source: wifiManager.online ? "http://img3.imageshack.us/img3/9870/magepicture.jpg" : ""
        anchors.bottom: parent.bottom

        width: parent.width
        height: sourceSize.height * (width / sourceSize.width);
    }
}
