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
import "../common"

Item {
    id: root
    property string title: qsTr("Network Settings")

    Column {
        id: networkSettingsColumn
        spacing: pluginMain.spacing
        anchors.margins: viewSettings.pageMargin

        Row {
            spacing: root.width * 0.025
            leftPadding: pluginMain.margin
            Text {
                text: qsTr("Wi-Fi")
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: pluginMain.subTitleFontSize
                font.family: appFont
                color: "white"
            }

            CustomSwitch {
                id: wifiSwitch
                indicatorWidth: root.width * 0.15
                indicatorHeight: root.height * 0.06
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
        }

        Text {
            text: qsTr("Network list:")
            font.pixelSize: pluginMain.subTitleFontSize
            font.family: appFont
            font.styleName: "SemiBold"
            color: "white"
        }

        Row {
            id: listViewRow
            leftPadding: pluginMain.margin
            NetworkListView {
                id: networkList
                width: root.width - listViewRow.leftPadding
                height: root.height - listViewRow.y - networkSettingsColumn.anchors.margins
            }
        }
    }
}
