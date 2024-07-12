// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtDeviceUtilities.NetworkSettings

Item {
    id: root
    property string title: qsTr("Ethernet Settings")
    property var service: undefined
    property bool ipv4Changed: false
    property bool ipv6Changed: false
    property bool nameserversChanged: false
    property bool domainsChanged: false
    property bool proxyChanged: false

    Component.onDestruction: {
        //Clear all unsaved changes from models
        service.proxy.excludes.resetChanges();
        service.nameservers.resetChanges();
        service.domains.resetChanges();
    }

    Component.onCompleted: titlebar.title = service.name;

    Flickable {
        anchors.fill: parent
        anchors.top: saveButton.bottom
        anchors.margins: 20
        contentHeight: content.height
        contentWidth: width

        Column {
            id: content
            width: parent.width
            spacing: 10
            property int titleWidth: width * 0.382
            property int groupWidth: width - saveButton.width - 10

            //IPv4 Config
            GroupBox {
                width: content.groupWidth
                Layout.fillWidth: true
                title: qsTr("IPv4")

                ColumnLayout {
                    anchors.fill: parent

                    ComboBoxEntry {
                        id: ipv4Method
                        title:  qsTr("Connection method:")
                        titleWidth: content.titleWidth
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
                        onCurrentIndexChanged: {
                            ipv4Changed = true;
                            service.ipv4.method = model.get(currentIndex).method;
                        }
                    }

                    GridLayout {
                        columnSpacing: 10
                        rows: 3
                        columns: 2
                        visible: service.ipv4.method !== NetworkSettingsIPv4.Off
                        width: parent.width

                        Label {
                            text: qsTr("Address:")
                            Layout.preferredWidth: content.titleWidth
                            horizontalAlignment: Text.AlignRight
                        }
                        IpAddressTextField {
                            id: ipv4Address
                            text: service.ipv4.address
                            onTextChanged: ipv4Changed = true;
                            onAccepted: if (text.length > 0) service.ipv4.address = text
                            enabled: service.ipv4.method === NetworkSettingsIPv4.Manual
                        }
                        Label {
                            text: qsTr("Mask:")
                            Layout.preferredWidth: content.titleWidth
                            horizontalAlignment: Text.AlignRight
                        }
                        IpAddressTextField {
                            id: ipv4Mask
                            text: service.ipv4.mask
                            onTextChanged: ipv4Changed = true;
                            onAccepted: if (text.length > 0) service.ipv4.mask = text
                            enabled: service.ipv4.method === NetworkSettingsIPv4.Manual
                        }
                        Label {
                            text: qsTr("Router:")
                            Layout.preferredWidth: content.titleWidth
                            horizontalAlignment: Text.AlignRight
                        }
                        IpAddressTextField {
                            id: ipv4Gateway
                            text: service.ipv4.gateway
                            onTextChanged: ipv4Changed = true
                            onAccepted: if (text.length > 0) service.ipv4.gateway = text
                            enabled: service.ipv4.method === NetworkSettingsIPv4.Manual
                        }
                    }
                }
            }

            //IPv6 Config
            GroupBox {
                width: content.groupWidth
                Layout.fillWidth: true
                title: qsTr("IPv6")

                ColumnLayout {
                    anchors.fill: parent

                    ComboBoxEntry {
                        id: ipv6Method
                        title: qsTr("Connection method:")
                        titleWidth: content.titleWidth
                        model: ListModel {
                            id: ipv6methodsmodel

                            ListElement {
                                text: qsTr("Auto")
                                method: NetworkSettingsIPv6.Auto
                            }
                            ListElement {
                                text: qsTr("Manual")
                                method: NetworkSettingsIPv6.Manual
                            }
                            ListElement {
                                text: qsTr("Off")
                                method: NetworkSettingsIPv6.Off
                            }
                        }

                        onCurrentIndexChanged: {
                            ipv6Changed = true;
                            service.ipv6.method = model.get(currentIndex).method;
                        }
                    }
                    GridLayout {
                        columnSpacing: 10
                        rows: 3
                        columns: 2
                        visible: service.ipv6.method !== NetworkSettingsIPv6.Off

                        Label {
                            text: qsTr("Address:")
                            Layout.preferredWidth: content.titleWidth
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            id: ipv6Address
                            text: service.ipv6.address
                            Layout.fillWidth: true
                            onTextChanged: ipv6Changed = true;
                            onAccepted: if (text.length > 0) service.ipv6.address = text
                            enabled: service.ipv6.method === NetworkSettingsIPv6.Manual
                        }
                        Label {
                            text: qsTr("Router:")
                            Layout.preferredWidth: content.titleWidth
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            id: ipv6Gateway
                            text: service.ipv6.gateway
                            Layout.fillWidth: true
                            onTextChanged: ipv6Changed = true;
                            onAccepted: if (text.length > 0) service.ipv4.gateway = text
                            enabled: service.ipv6.method === NetworkSettingsIPv6.Manual
                        }
                        Label {
                            text: qsTr("Prefix length:")
                            Layout.preferredWidth: content.titleWidth
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            id: ipv6PrefixLength
                            text: service.ipv6.prefixLength
                            Layout.fillWidth: true
                            validator: IntValidator { bottom: 0; top: 255 }
                            onTextChanged: ipv6Changed = true
                            onAccepted: if (text.length > 0) service.ipv6.prefixLength = parseInt(text)
                            enabled: service.ipv6.method === NetworkSettingsIPv6.Manual
                        }
                    }
                }
            }
            GroupBox {
                width: content.groupWidth
                Layout.fillWidth: true
                title: qsTr("Name servers")

                ColumnLayout {
                    anchors.fill: parent
                    AddressListEntry {
                        title: qsTr("Address:")
                        model: service.nameservers
                        modified: nameserversChanged
                    }
                }
            }
            GroupBox {
                width: content.groupWidth
                Layout.fillWidth: true
                title: qsTr("Domains")

                ColumnLayout {
                    anchors.fill: parent
                    AddressListEntry {
                        title: qsTr("Address:")
                        model: service.domains
                        modified: domainsChanged
                    }
                }
            }
            GroupBox {
                width: content.groupWidth
                Layout.fillWidth: true
                title: qsTr("Proxy Settings")

                ColumnLayout {
                    id: proxyLayout
                    width: parent.width

                    ComboBoxEntry {
                        title: qsTr("Configuration:")
                        titleWidth: content.titleWidth
                        model: ListModel {
                            id: proxyMethodModel

                            ListElement {
                                text: qsTr("Direct")
                                method: NetworkSettingsProxy.Direct
                            }
                            ListElement {
                                text: qsTr("Auto")
                                method: NetworkSettingsProxy.Auto
                            }
                            ListElement {
                                text: qsTr("Manual")
                                method: NetworkSettingsProxy.Manual
                            }
                        }
                        Component.onCompleted: currentIndex = service.proxy.method

                        onCurrentIndexChanged: {
                            proxyChanged = true;
                            service.proxy.method = model.get(currentIndex).method;
                        }
                    }
                    RowLayout {
                        spacing: 10
                        visible: service.proxy.method !== NetworkSettingsProxy.Direct

                        Label {
                            text: service.proxy.method === NetworkSettingsProxy.Manual ? qsTr("Proxy address:") : qsTr("Configuration URL:")
                            Layout.preferredWidth: content.titleWidth
                            horizontalAlignment: Text.AlignRight
                            Layout.alignment: Qt.AlignVCenter
                            elide: Label.ElideRight
                        }
                        TextField {
                            id: proxyUrl
                            text: service.proxy.url
                            Layout.fillWidth: true
                            onTextChanged: proxyChanged = true;
                            onAccepted: service.proxy.url = text;
                        }
                    }
                    AddressListEntry {
                        model: service.proxy.excludes
                        modified: proxyChanged
                        title: qsTr("No proxy for:")
                        visible: service.proxy.method === NetworkSettingsProxy.Manual
                    }
                }
            }
        }
    }
    Button {
        id: saveButton
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 20
        text: qsTr("Save")

        onClicked: {
            if (ipv4Changed) {
                ipv4Address.accepted();
                ipv4Mask.accepted();
                ipv4Gateway.accepted();
                service.setupIpv4Config();
            }

            if (ipv6Changed) {
                ipv6Address.accepted();
                ipv6Gateway.accepted();
                ipv6PrefixLength.accepted();
                service.setupIpv6Config();
            }

            if (nameserversChanged) {
                service.setupNameserversConfig();
            }

            if (domainsChanged) {
                service.setupDomainsConfig();
            }

            if (proxyChanged) {
                proxyUrl.accepted();
                service.setupNetworkSettingsProxy();
            }
            stackView.pop();
        }
    }
    onServiceChanged: {
        ipv4Method.currentIndex = service.ipv4.method
        ipv6Method.currentIndex = service.ipv6.method
    }
}
