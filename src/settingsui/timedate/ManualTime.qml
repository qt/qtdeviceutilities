/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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
import QtQuick.Controls 2.0
import QtDeviceUtilities.QtButtonImageProvider 1.0
import QtDeviceUtilities.TimeDateSettings 1.0

Item {
    id: root
    property int margin: root.width * 0.05

    property int firstYear: 2017

    function zeroPadTime(timeToPad) {
        return timeToPad < 10 ? "0" + timeToPad : timeToPad
    }

    Column {
        spacing: pluginMain.spacing

        Text {
            color: "white"
            text: qsTr("Set Date")
            font.pixelSize: pluginMain.subTitleFontSize
            font.family: appFont
        }

        // Row of date comboboxes
        Row {
            spacing: pluginMain.spacing
            leftPadding: pluginMain.margin

            CustomComboBox {
                id: dayBox
                width: root.width * 0.15
                height: pluginMain.buttonHeight
                displayText: currentIndex + 1
                property var date: new Date(firstYear, 1, 0)

                model: 31
                itemsVisible: 10

                delegate: ItemDelegate {
                    id: dayDelegate
                    height: dayBox.height
                    contentItem: Text {
                        anchors.left: dayDelegate.left
                        anchors.leftMargin: pluginMain.margin
                        text: modelData + 1
                        color: dayBox.currentIndex == index ? "#41cd52" : "white"
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: pluginMain.valueFontSize
                        font.family: appFont
                        font.styleName: "Regular"
                    }
                }

                property int month: monthBox.currentIndex + 1
                onMonthChanged: updateDate()

                property int year: yearBox.currentIndex + firstYear
                onYearChanged: updateDate()

                function updateDate() {
                    date = new Date(year, month, 0)
                }
            }

            CustomComboBox {
                id: monthBox
                width: root.width * 0.35
                height: pluginMain.buttonHeight
                model: ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"]
                itemsVisible: 12
                delegate: ItemDelegate {
                    id: monthDelegate
                    height: monthBox.height
                    contentItem: Text {
                        anchors.left: monthDelegate.left
                        anchors.leftMargin: pluginMain.margin
                        color: monthBox.currentIndex == index ? "#41cd52" : "white"
                        elide: Text.ElideRight
                        text: modelData
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: pluginMain.valueFontSize
                        font.family: appFont
                        font.styleName: "Regular"

                    }
                }
            }
            CustomComboBox {
                id: yearBox
                width: root.width * 0.2
                height: pluginMain.buttonHeight
                displayText: yearBox.currentIndex + date.getFullYear()
                property var date: new Date()

                model: 50
                itemsVisible: 8
                delegate: ItemDelegate {
                    id: yearDelegate
                    height: yearBox.height
                    contentItem: Text {
                        anchors.left: yearDelegate.left
                        anchors.leftMargin: pluginMain.margin
                        text: index + firstYear
                        color: yearBox.currentIndex == index ? "#41cd52" : "white"
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: pluginMain.valueFontSize
                        font.family: appFont
                        font.styleName: "Regular"
                    }
                }
            }
        } // Row of date comboboxes

        Text {
            color: "white"
            text: qsTr("Set Time")
            font.pixelSize: pluginMain.subTitleFontSize
            font.family: appFont
        }

        // Row of time comboboxes
        Row {
            spacing: pluginMain.spacing
            leftPadding: pluginMain.margin

            CustomComboBox {
                id: hourBox
                width: root.width * 0.15
                height: pluginMain.buttonHeight
                displayText: zeroPadTime(hour)
                property int hour: currentIndex

                model: 24
                itemsVisible: 8

                delegate: ItemDelegate {
                    id: hourDelegate
                    height: hourBox.height
                    contentItem: Text {
                        anchors.left: hourDelegate.left
                        anchors.leftMargin: pluginMain.margin
                        text: zeroPadTime(parseInt(modelData))
                        color: hourBox.currentIndex == index ? "#41cd52" : "white"
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: pluginMain.valueFontSize
                        font.family: appFont
                        font.styleName: "Regular"
                    }
                }
            }
            CustomComboBox {
                id: minuteBox
                width: root.width * 0.15
                height: pluginMain.buttonHeight
                displayText: zeroPadTime(minute)
                property int minute: currentIndex

                model: 60
                itemsVisible: 8

                delegate: ItemDelegate {
                    id: minuteDelegate
                    height: minuteBox.height
                    contentItem: Text {
                        anchors.left: minuteDelegate.left
                        anchors.leftMargin: pluginMain.margin
                        text: zeroPadTime(parseInt(modelData))
                        color: minuteBox.currentIndex == index ? "#41cd52" : "white"
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: pluginMain.valueFontSize
                        font.family: appFont
                        font.styleName: "Regular"
                    }
                }
            }
        } // Row of time comboboxes

        // Row of set/cancel buttons
        Row {
            spacing: pluginMain.spacing

            QtButton {
                id: dateSetButton
                height: pluginMain.buttonHeight
                fillColor: "#41cd52"
                borderColor: "transparent"
                text: qsTr("SET")

                onClicked: {
                    var currentTime = TimeManager.time;
                    var newDate = new Date();

                    newDate.setFullYear(dayBox.year)
                    newDate.setMonth(dayBox.month - 1)
                    newDate.setDate(dayBox.currentIndex+1)
                    newDate.setHours(hourBox.hour)
                    newDate.setMinutes(minuteBox.minute)
                    newDate.setSeconds(currentTime.getSeconds())
                    TimeManager.ntp = false
                    TimeManager.time = newDate;
                    console.log("newDate: " + newDate)
                    console.log("TimeManager.time: " + TimeManager.time)
                }
            }
            QtButton {
                id: dateCancelButton
                height: pluginMain.buttonHeight
                fillColor: "#9d9faa"
                borderColor: "transparent"
                text: qsTr("CANCEL")
                onClicked: {
                    settingsLoader.source = "qrc:/timedate/TimeDate.qml"
                }
            }
        } // Row of set/cancel buttons
    } // Main column
}
