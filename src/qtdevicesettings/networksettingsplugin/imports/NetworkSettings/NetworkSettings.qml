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
    property string title: qsTr("Network Settings")
    GroupBox {
        id: networkSelect
        anchors.fill: parent
        anchors.rightMargin: parent.width * 0.618
        anchors.leftMargin: Math.round(20 * Flat.FlatStyle.scaleFactor)
        anchors.topMargin: Math.round(20 * Flat.FlatStyle.scaleFactor)
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
            width: networkSelect.width
            ExclusiveGroup {
                id: exgroup
            }

            Repeater {
                model: NetworkSettingsManager.networks

                Button {
                    implicitWidth: networkSelect.width - Math.round(20 * Flat.FlatStyle.scaleFactor)
                    implicitHeight: Math.round(54 * Flat.FlatStyle.scaleFactor)
                    checkable: true
                    exclusiveGroup: exgroup

                    Row {
                        anchors.fill: parent
                        spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)

                        Image {
                           id: typeId
                           height: parent.height
                           width: height
                        }

                        Column {
                            height: Math.round(parent.height * 0.7)
                            spacing: 0
                            anchors.verticalCenter: parent.verticalCenter
                            TextLabel {
                                text: name
                            }

                            TextLabel {
                                text: networkSelect.stateToStr(status)
                                font.pixelSize: Math.round(12 * Flat.FlatStyle.scaleFactor)
                            }
                        }
                    }

                    onCheckedChanged: {
                        if (checked) {
                            networkDetails.service = NetworkSettingsManager.getService(name, type);
                            if (type === NetworkType.Wired) {
                                networkDetails.source = "WiredSettings.qml";
                            }
                            else if (type === NetworkType.Wifi) {
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
        anchors.fill: parent
        anchors.leftMargin: networkSelect.width + Math.round(20 * Flat.FlatStyle.scaleFactor)
    }
}
