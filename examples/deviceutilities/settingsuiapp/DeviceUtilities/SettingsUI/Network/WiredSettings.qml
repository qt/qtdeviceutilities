// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtDeviceUtilities.NetworkSettings

Item {
    id: root
    anchors.fill: parent
    property bool editMode: false
    property var service: null
    property string title: qsTr("Network");

    Component.onCompleted: {
        NetworkSettingsManager.services.type = NetworkSettingsType.Wired;
        root.service = NetworkSettingsManager.services.itemFromRow(0);
        ipv4Method.currentIndex = service.ipv4.method
    }

    onServiceChanged: {
        NetworkSettingsManager.services.type = NetworkSettingsType.Wired;
        if (!root.service) {
            root.service = NetworkSettingsManager.services.itemFromRow(0);
        }
    }

    GroupBox {
        title: qsTr("Ethernet Connection")
        anchors.fill: parent

        Column {
            spacing: 10
            width: parent.width

            ComboBoxEntry {
                id: ipv4Method
                title: qsTr("Connection method:")
                model: ListModel {
                    id: methodsModel

                    ListElement {
                        text: "DHCP"
                        method: NetworkSettingsIPv4.Dhcp
                    }
                    ListElement {
                        text: "Manual"
                        method: NetworkSettingsIPv4.Manual
                    }
                    ListElement {
                        text: "Off"
                        method: NetworkSettingsIPv4.Off
                    }
                }

                Component.onCompleted: currentIndex = service.ipv4.method
                onCurrentIndexChanged: {
                    if (model.get(currentIndex).method !== NetworkSettingsIPv4.Dhcp) {
                        service.ipv4.method = model.get(currentIndex).method;
                        editMode = true;
                    }
                    else if (service){
                        //Enable DHCP
                        if (service.ipv4.method !== model.get(currentIndex).method) {
                            service.ipv4.method = model.get(currentIndex).method;
                            service.setupIpv4Config();
                        }
                        editMode = false;
                    }
                }
            }
            Item {
                width: parent.width
                height: gridLayout.implicitHeight
                GridLayout {
                    id: gridLayout
                    columns: 2
                    rows: 4
                    width: parent.width
                    visible: service.ipv4.method !== NetworkSettingsIPv4.Off

                    Label {
                        text: qsTr("IP Address: ")
                        width: parent.width * .3
                    }
                    IpAddressTextField {
                        id: ipv4Address
                        text: service.ipv4.address
                        enabled: editMode
                        onAccepted: if (text.length > 0) service.ipv4.address = text
                    }
                    Label {
                        text: qsTr("Mask: ")
                    }
                    IpAddressTextField {
                        id: ipv4Mask
                        text: service.ipv4.mask
                        enabled: editMode
                        onAccepted: if (text.length > 0) service.ipv4.mask = text
                    }
                    Label {
                        text: qsTr("Router: ")
                    }
                    IpAddressTextField {
                        id: ipv4Gateway
                        text: service.ipv4.gateway
                        enabled: editMode
                        onAccepted: if (text.length > 0) service.ipv4.gateway = text
                    }
                    Label {
                        Layout.alignment: Qt.AlignTop
                        text: qsTr("DNS server: ")
                    }
                    Column {
                        spacing: 10
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignTop

                        Repeater {
                            model: service.nameservers

                            Label {
                                text: display
                            }
                        }
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    enabled: !editMode
                             && methodsModel.get(ipv4Method.currentIndex).method === NetworkSettingsIPv4.Manual
                    onClicked: editMode = true
                }
            }
            Row {
                spacing: 10
                Button {
                    text: qsTr("Save")
                    visible: editMode

                    onClicked: {
                        ipv4Address.accepted();
                        ipv4Mask.accepted();
                        ipv4Gateway.accepted();
                        service.setupIpv4Config();
                        editMode = false;
                    }
                }
                Button {
                    text: qsTr("Cancel")
                    visible: editMode
                    onClicked: {
                        editMode = false;
                        ipv4Method.currentIndex = service.ipv4.method
                    }
                }
            }
            Button {
                text: qsTr("Edit")
                visible: !editMode
                onClicked: stackView.push(Qt.resolvedUrl("EditWiredSettings.qml"), {service: root.service});
            }
        }
    }
}
