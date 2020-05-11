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
import QtDeviceUtilities.TimeDateSettings 1.0
import QtDeviceUtilities.LocaleSettings 1.0
import QtDeviceUtilities.QtButtonImageProvider 1.0

Item {
    id: root
    property int margin: root.width * 0.05

    Column {
        spacing: pluginMain.spacing
        anchors.top: parent.top

        // Display current date
        Text {
            color: "white"
            text: qsTr("Current date")
            font.pixelSize: pluginMain.subTitleFontSize
            font.family: appFont
        }

        Text {
            id: dateText
            leftPadding: pluginMain.margin
            color: "white"
            font.pixelSize: pluginMain.valueFontSize
            font.family: appFont
            Timer {
                id: dateTimer
                interval: 1000
                running: true
                repeat: true
                triggeredOnStart: true
                onTriggered: {
                    var date = new Date();
                    dateText.text = date.toLocaleString(Qt.locale(LocaleManager.locale), Locale.LongFormat)
                }
            }
        }

        // Set date automatically / manually
        Text {
            color: "white"
            text: qsTr("Set Date & Time")
            font.pixelSize: pluginMain.subTitleFontSize
            font.family: appFont
        }

        Row {
            spacing: pluginMain.spacing
            leftPadding: pluginMain.margin
            QtButton {
                id: automaticButton
                height: pluginMain.buttonHeight
                text: qsTr("AUTOMATICALLY")
                onClicked: TimeManager.ntp = true
            }
            QtButton {
                id: manualButton
                height: pluginMain.buttonHeight
                fillColor: viewSettings.buttonGrayColor
                borderColor: "transparent"
                text: qsTr("MANUALLY")
                onClicked: {
                    settingsLoader.source = "qrc:/timedate/ManualTime.qml"
                }
            }
        }

        // Select timezone
        Text {
            color: "white"
            text: qsTr("Time Zone")
            font.pixelSize: pluginMain.subTitleFontSize
            font.family: appFont
        }

        Row {
            leftPadding: pluginMain.margin
            CustomComboBox {
                id: timeZoneBox
                width: automaticButton.width + manualButton.width + pluginMain.spacing
                height: pluginMain.buttonHeight
                textRole: "id"
                itemsVisible: 7
                currentIndex: -1
                model: TimezonesFilter
                delegate: ItemDelegate {
                    id: timeZoneDelegate
                    height: timeZoneBox.height
                    width: timeZoneBox.width
                    contentItem: Text {
                        anchors.left: timeZoneDelegate.left
                        anchors.leftMargin: pluginMain.margin
                        text: modelData["id"]
                        color: timeZoneBox.currentIndex == index ? viewSettings.buttonGreenColor : "white"
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: pluginMain.valueFontSize
                        font.family: appFont
                        font.styleName: "Regular"
                    }
                }

                Component.onCompleted: {
                    var n = TimezonesFilter.indexForTimezone(TimeManager.timeZone)
                    timeZoneBox.currentIndex = n
                }

                Connections {
                    target: TimezonesFilter.sourceModel
                    function onReady() {
                        var n = TimezonesFilter.indexForTimezone(TimeManager.timeZone)
                        timeZoneBox.currentIndex = n
                    }
                }

                onCurrentIndexChanged: {
                    var val = TimezonesFilter.itemFromRow(currentIndex);
                    if (val && val !== "") {
                        TimeManager.timeZone = val
                    }
                }
            }
        }

        Component {
            id: zoneselect
            TimezonesView {}
        }
    }
}
