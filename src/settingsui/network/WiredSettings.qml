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
import QtQuick.Controls 2.0
import QtDeviceUtilities.NetworkSettings 1.0

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
            GridLayout {
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
