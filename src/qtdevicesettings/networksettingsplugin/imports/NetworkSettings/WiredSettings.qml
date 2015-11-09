/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles.Flat 1.0 as Flat
import com.theqtcompany.settings.common 1.0
import com.theqtcompany.settings.network 1.0

Item {
    id: root
    anchors.fill: parent
    anchors.margins: Math.round(20 * Flat.FlatStyle.scaleFactor)
    property bool editMode: false

    onVisibleChanged: {
        if (visible)
            titlebar.title = qsTr("Network");
    }

    ListModel {
        id: methodsModel

        ListElement {
            text: "DHCP"
            method: IPv4Config.Dhcp
        }
        ListElement {
            text: "Manual"
            method: IPv4Config.Manual
        }
        ListElement {
            text: "Off"
            method: IPv4Config.Off
        }
    }

    GroupBox {
        title: qsTr("Ethernet Connection")
        anchors.fill: parent
        Layout.fillWidth: true
        flat: true
        Column {
            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
            Row {
                TextLabel {
                    text: qsTr("Connection method: ")
                }

                CustomCombobox {
                    id: ipv4Method
                    model: methodsModel
                    Component.onCompleted: selectedIndex = service.ipv4.method
                    onSelectedIndexChanged : {
                        if (model.get(selectedIndex).method !== IPv4Config.Dhcp) {
                            service.ipv4.method = model.get(selectedIndex).method;
                            editMode = true;
                        }
                        else {
                            //Enable DHCP
                            if (service.ipv4.method !== model.get(selectedIndex).method) {
                                service.ipv4.method = model.get(selectedIndex).method;
                                service.setupIpv4Config();
                            }

                            editMode = false;
                        }
                    }
                    delegate: FlatStyledDropdownDelegate { }
                }
            }

            Row {
                visible: service.ipv4.method !== IPv4Config.Off
                TextLabel {
                    text: qsTr("IP Address: ")
                }
                TextLabel {
                    text: service.ipv4.address
                    visible: !editMode
                }
                IpAddressTextField {
                    id: ipv4Address
                    placeholderText: service.ipv4.address
                    visible: editMode
                    onAccepted: if (text.length > 0) service.ipv4.address = text
                }
            }

            Row {
                visible: service.ipv4.method !== IPv4Config.Off
                TextLabel {
                    text: qsTr("Mask: ")
                }
                TextLabel {
                    text: service.ipv4.mask
                    visible: !editMode
                }
                IpAddressTextField {
                    id: ipv4Mask
                    placeholderText: service.ipv4.mask
                    visible: editMode
                    onAccepted: if (text.length > 0) service.ipv4.mask = text
                }
            }

            Row {
                visible: service.ipv4.method !== IPv4Config.Off
                TextLabel {
                    text: qsTr("Router: ")
                }
                TextLabel {
                    text: service.ipv4.gateway
                    visible: !editMode
                }
                IpAddressTextField {
                    id: ipv4Gateway
                    placeholderText: service.ipv4.gateway
                    visible: editMode
                    onAccepted: if (text.length > 0) service.ipv4.gateway = text
                }
            }

            Row {
                visible: service.ipv4.method !== IPv4Config.Off
                TextLabel {
                    text: qsTr("DNS server: ")
                }
                Row {
                    spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                    Repeater {
                        model: service.nameservers

                        TextLabel {
                            text: display
                        }
                    }
                }
            }

            Row {
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
                        methodSelection.currentIndex = service.ipv4.method
                    }
                }
            }
            Button {
                text: qsTr("Edit")
                visible: !editMode

                onClicked: stackView.push( { item : Qt.resolvedUrl("EditWiredSettings.qml"),  properties : { service: service } } )
            }
        }
    }
}

