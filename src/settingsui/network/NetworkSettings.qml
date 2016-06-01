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
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtDeviceUtilities.NetworkSettings 1.0

Item {
    property string title: qsTr("Network Settings")
    GroupBox {
        id: networkSelect
        anchors.rightMargin: parent.width * 0.618
        anchors.leftMargin: 20
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        anchors.fill: parent
        title: qsTr("Select Connection")

        function stateToStr(serviceState) {
            switch (serviceState) {
            case NetworkService.Idle:
                return qsTr("Idle");
            case NetworkService.Failure:
                return qsTr("Failure");
            case NetworkService.Association:
                return qsTr("Association");
            case NetworkService.Configuration:
                return qsTr("Configuration");
            case NetworkService.Ready:
                return qsTr("Ready");
            case NetworkService.Disconnect:
                return qsTr("Disconnect");
            case NetworkService.Online:
                return qsTr("Online");
            default:
                return qsTr("Undefined");
            }
        }
        ColumnLayout {
            width: parent.width

            Repeater {
                model: NetworkSettingsManager.interfaces

                RadioButton {
                    id: btn
                    Layout.fillWidth: true
                    implicitHeight: 54

                    contentItem: Rectangle {
                        color: checked ? "#80c342" : "white"
                        border.color: "#bdbebf"
                        border.width: 1
                        anchors.fill: parent

                        Row {
                            anchors.fill: parent
                            spacing: 10

                            Image {
                                id: typeId
                                height: parent.height
                                width: height
                            }
                            Column {
                                height: parent.height * .7
                                spacing: 0
                                anchors.verticalCenter: parent.verticalCenter
                                Label {
                                    text: name
                                }
                                Label {
                                    text: networkSelect.stateToStr(modelData.state)
                                    font.pixelSize: 12
                                }
                            }
                        }
                    }

                    onCheckedChanged: {
                        if (checked) {
                            networkDetails.selectedInterface = modelData
                            networkDetails.selectedInterface.scanServices()
                            if (type === NetworkSettingsType.Wired) {
                                networkDetails.source = "WiredSettings.qml";
                            }
                            else if (type === NetworkSettingsType.Wifi) {
                                networkDetails.source = "WifiSettings.qml";
                            }
                        }
                    }
                }
            }
        }
    }

    NetworkDetails {
        id: networkDetails
        anchors.leftMargin: networkSelect.width + 40
        anchors.topMargin: 20
        anchors.rightMargin: 20
        anchors.bottomMargin: 20
        anchors.fill: parent
    }
}
