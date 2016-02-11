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
import "../common"
import com.theqtcompany.settings.network 1.0

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
        anchors.margins: Math.round(40 * Flat.FlatStyle.scaleFactor)
        anchors.bottomMargin: Math.round(20 * Flat.FlatStyle.scaleFactor)
        contentHeight: content.height
        contentWidth: width
        interactive: !ipv4Method.popupVisible && !ipv6Method.popupVisible && !proxyMethodSel.popupVisible
        Column {
            id: content
            width: parent.width
            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
            property int titleWidth: Math.round(width * 0.382)
            property int groupWidth: Math.round(width - saveButton.width - 10 * Flat.FlatStyle.scaleFactor)

            //IPv4 Config
            GroupBox {
                flat: false
                width: content.groupWidth
                Layout.fillWidth: true
                title: qsTr("IPv4")

                ColumnLayout {
                    anchors.fill: parent

                    ListModel {
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

                    Row {
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        TextLabel {
                            text: qsTr("Connection method")
                            width: content.titleWidth
                            horizontalAlignment: Text.AlignRight
                        }

                        CustomCombobox {
                            id: ipv4Method
                            model: methodsModel
                            Component.onCompleted: selectedIndex = service.ipv4.method

                            onSelectedIndexChanged : {
                                ipv4Changed = true;
                                service.ipv4.method = model.get(selectedIndex).method;
                            }
                            delegate: FlatStyledDropdownDelegate { }
                        }
                    }

                    Column {
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        visible: service.ipv4.method === NetworkSettingsIPv4.Dhcp

                        Row {
                            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                            TextLabel {
                                text: qsTr("Address")
                                width: content.titleWidth
                                horizontalAlignment: Text.AlignRight
                            }
                            TextLabel {
                                text: service.ipv4.address
                            }
                        }

                        Row {
                            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                            TextLabel {
                                text: qsTr("Mask")
                                width: content.titleWidth
                                horizontalAlignment: Text.AlignRight
                            }
                            TextLabel {
                                text: service.ipv4.mask
                            }
                        }

                        Row {
                            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                            TextLabel {
                                text: qsTr("Router")
                                width: content.titleWidth
                                horizontalAlignment: Text.AlignRight
                            }
                            TextLabel {
                                text: service.ipv4.gateway
                            }
                        }
                    }

                    Column {
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        visible: service.ipv4.method === NetworkSettingsIPv4.Manual

                        Row {
                            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                            TextLabel {
                                text: qsTr("Address")
                                width: content.titleWidth
                                horizontalAlignment: Text.AlignRight
                            }
                            IpAddressTextField {
                                id: ipv4Address
                                text: service.ipv4.address
                                onTextChanged: ipv4Changed = true;

                                onAccepted: if (text.length > 0) service.ipv4.address = text
                            }
                        }

                        Row {
                            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                            TextLabel {
                                text: qsTr("Mask")
                                width: content.titleWidth
                                horizontalAlignment: Text.AlignRight
                            }
                            IpAddressTextField {
                                id: ipv4Mask
                                text: service.ipv4.mask
                                onTextChanged: ipv4Changed = true;
                                onAccepted: if (text.length > 0) service.ipv4.mask = text
                            }
                        }

                        Row {
                            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                            TextLabel {
                                text: qsTr("Router")
                                width: content.titleWidth
                                horizontalAlignment: Text.AlignRight
                            }
                            IpAddressTextField {
                                id: ipv4Gateway
                                text: service.ipv4.gateway

                                onTextChanged: ipv4Changed = true

                                onAccepted: if (text.length > 0) service.ipv4.gateway = text
                            }
                        }
                    }
                }
            }

            //IPv6 Config
            GroupBox {
                flat: false
                width: content.groupWidth
                Layout.fillWidth: true
                title: qsTr("IPv6")

                ColumnLayout {
                    anchors.fill: parent

                    ListModel {
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

                    Row {
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        TextLabel {
                            text: qsTr("Connection method: ")
                            width: content.titleWidth
                            horizontalAlignment: Text.AlignRight
                        }

                        CustomCombobox {
                            id: ipv6Method
                            model: ipv6methodsmodel
                            Component.onCompleted: selectedIndex = service.ipv6.method
                            onSelectedIndexChanged : {
                                ipv6Changed = true;
                                service.ipv6.method = model.get(selectedIndex).method;
                            }
                            delegate: FlatStyledDropdownDelegate { }
                        }
                    }

                    Column {
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        visible: service.ipv6.method === NetworkSettingsIPv6.Auto
                        Row {
                            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                            TextLabel {
                                text: qsTr("Address")
                                width: content.titleWidth
                                horizontalAlignment: Text.AlignRight
                            }
                            TextLabel {
                                text: service.ipv6.address
                            }
                        }

                        Row {
                            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                            TextLabel {
                                text: qsTr("Router")
                                width: content.titleWidth
                                horizontalAlignment: Text.AlignRight
                            }
                            TextLabel {
                                text: service.ipv6.gateway
                            }
                        }

                        Row {
                            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                            TextLabel {
                                text: qsTr("Prefix length")
                                width: content.titleWidth
                                horizontalAlignment: Text.AlignRight
                            }
                            TextLabel {
                                text: service.ipv6.prefixLength
                            }
                        }
                    }

                    Column {
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        visible: service.ipv6.method === NetworkSettingsIPv6.Manual
                        Row {
                            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                            TextLabel {
                                text: qsTr("Address")
                                width: content.titleWidth
                                horizontalAlignment: Text.AlignRight
                            }
                            TextField {
                                id: ipv6Address
                                text: service.ipv6.address
                                onTextChanged: ipv6Changed = true;
                                onAccepted: if (text.length > 0) service.ipv6.address = text
                            }
                        }

                        Row {
                            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                            TextLabel {
                                text: qsTr("Router")
                                width: content.titleWidth
                                horizontalAlignment: Text.AlignRight
                            }
                            TextField {
                                id: ipv6Gateway
                                text: service.ipv6.gateway
                                onTextChanged: ipv6Changed = true;
                                onAccepted: if (text.length > 0) service.ipv4.gateway = text
                            }
                        }

                        Row {
                            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                            TextLabel {
                                text: qsTr("Prefix length")
                                width: content.titleWidth
                                horizontalAlignment: Text.AlignRight
                            }
                            TextField {
                                id: ipv6PrefixLength
                                text: service.ipv6.prefixLength
                                validator: IntValidator { bottom: 0; top: 255 }

                                onTextChanged: ipv6Changed = true

                                onAccepted: if (text.length > 0) service.ipv6.prefixLength = parseInt(text)
                            }
                        }
                    }
                }
            }

            GroupBox {
                flat: false
                width: content.groupWidth
                Layout.fillWidth: true
                title: qsTr("Name servers")

                ColumnLayout {
                    anchors.fill: parent
                    Row {
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        TextLabel {
                            text: qsTr("Address")
                            width: content.titleWidth
                            horizontalAlignment: Text.AlignRight
                        }
                        Column {
                            Repeater {
                                model: service.nameservers

                                Row {
                                    TextField {
                                        text: edit
                                        onTextChanged: {
                                            nameserversChanged = true;
                                            edit = text;
                                        }
                                    }
                                    Button {
                                        implicitWidth: height
                                        text: "-"
                                        onClicked: {
                                            service.nameservers.remove(index);
                                            nameserversChanged = true;
                                        }
                                    }
                                    Button {
                                        implicitWidth: height
                                        visible: index==service.nameservers.count-1
                                        text: "+"
                                        onClicked: service.nameservers.append("")
                                    }
                                }
                            }
                        }
                        Row {
                            TextField {
                                id: nameServerEntryItem
                                visible: service.nameservers.count === 0
                                text: ""
                                onAccepted: service.nameservers.append(text)
                            }
                            Button {
                                implicitWidth: height
                                visible: nameServerEntryItem.visible
                                text: "+"
                                onClicked: nameServerEntryItem.accepted()
                            }
                        }
                    }
                }
            }

            GroupBox {
                flat: false
                width: content.groupWidth
                Layout.fillWidth: true
                title: qsTr("Domains")

                ColumnLayout {
                    anchors.fill: parent
                    Row {
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        TextLabel {
                            text: qsTr("Address")
                            width: content.titleWidth
                            horizontalAlignment: Text.AlignRight
                        }
                        Column {
                            Repeater {
                                model: service.domains
                                Row {
                                    TextField {
                                        text: edit
                                        onTextChanged: {
                                            root.domainsChanged = true;
                                            edit = text;
                                        }
                                    }
                                    Button {
                                        implicitWidth: height
                                        text: "-"
                                        onClicked: {
                                            service.domains.remove(index);
                                            domainsChanged.domainsChanged = true;
                                        }
                                    }
                                    Button {
                                        implicitWidth: height
                                        visible: index==service.domains.count-1
                                        text: "+"
                                        onClicked: service.domains.append("")
                                    }
                                }
                            }
                        }
                        Row {
                            TextField {
                                id: domainsEntryItem
                                visible: service.domains.count === 0
                                text: ""
                                onAccepted: service.domains.append(text)
                            }
                            Button {
                                implicitWidth: height
                                visible: domainsEntryItem.visible
                                text: "+"
                                onClicked: domainsEntryItem.accepted()
                            }
                        }
                    }
                }
            }

            GroupBox {
                flat: false
                width: content.groupWidth
                Layout.fillWidth: true
                title: qsTr("Proxy Settings")

                ColumnLayout {
                    anchors.fill: parent

                    ListModel {
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

                    Row {
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        TextLabel {
                            text: qsTr("Configuration")
                            width: content.titleWidth
                            horizontalAlignment: Text.AlignRight
                        }

                        CustomCombobox {
                            id: proxyMethodSel
                            model: proxyMethodModel
                            Component.onCompleted: selectedIndex = service.proxy.method
                            onSelectedIndexChanged : {
                                proxyChanged = true;
                                service.proxy.method = model.get(selectedIndex).method;
                            }
                            delegate: FlatStyledDropdownDelegate { }
                        }
                    }

                    Row {
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        visible: service.proxy.method !== NetworkSettingsProxy.Direct
                        TextLabel {
                            text: service.proxy.method === NetworkSettingsProxy.Manual ? qsTr("Proxy address") : qsTr("Automatic configuration URL")

                            width: content.titleWidth
                            horizontalAlignment: Text.AlignRight
                        }
                        TextField {
                            id: proxyUrl
                            text: service.proxy.url
                            onTextChanged: proxyChanged = true;
                            onAccepted: service.proxy.url = text;
                        }
                    }
                    Row {
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                        visible: service.proxy.method === NetworkSettingsProxy.Manual

                        TextLabel {
                            text: qsTr("No proxy for")
                            width: content.titleWidth
                            horizontalAlignment: Text.AlignRight
                        }

                        Column {
                            Repeater {
                                model: service.proxy.excludes

                                Row {
                                    TextField {
                                        text: edit
                                        onTextChanged: edit = text
                                    }
                                    Button {
                                        implicitWidth: height
                                        text: "-"
                                        onClicked: {
                                            service.proxy.excludes.remove(index);
                                            proxyChanged = true;
                                        }
                                    }
                                    Button {
                                        implicitWidth: height
                                        visible: index==service.proxy.excludes.count-1
                                        text: "+"
                                        onClicked: service.proxy.excludes.append("")
                                    }
                                }
                            }
                        }
                        Row {
                            TextField {
                                id: proxyEntryItem
                                visible: service.proxy.excludes.count === 0
                                text: ""
                                onAccepted: service.proxy.excludes.append(text)
                            }
                            Button {
                                implicitWidth: height
                                visible: proxyEntryItem.visible
                                text: "+"
                                onClicked: proxyEntryItem.accepted()
                            }
                        }
                    }
                }
            }
        }
    }

    Button {
        id: saveButton
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: Math.round(10 * Flat.FlatStyle.scaleFactor)
        anchors.rightMargin: Math.round(40 * Flat.FlatStyle.scaleFactor)
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
}
