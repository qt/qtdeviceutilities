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
    property var selectedDate: new Date()
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

                model: 31
                itemsVisible: 10
                currentIndex: selectedDate.getDate() - 1

                delegate: ItemDelegate {
                    id: dayDelegate
                    height: dayBox.height
                    width: dayBox.width
                    contentItem: Text {
                        anchors.left: dayDelegate.left
                        anchors.leftMargin: pluginMain.margin
                        text: modelData + 1
                        color: dayBox.currentIndex == index ? viewSettings.buttonGreenColor : "white"
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: pluginMain.valueFontSize
                        font.family: appFont
                        font.styleName: "Regular"
                    }
                }

                onCurrentIndexChanged: selectedDate.setDate(currentIndex + 1)
            }

            CustomComboBox {
                id: monthBox
                width: root.width * 0.35
                height: pluginMain.buttonHeight
                model: ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"]
                currentIndex: selectedDate.getMonth()

                itemsVisible: 12
                delegate: ItemDelegate {
                    id: monthDelegate
                    height: monthBox.height
                    width: monthBox.width
                    contentItem: Text {
                        anchors.left: monthDelegate.left
                        anchors.leftMargin: pluginMain.margin
                        color: monthBox.currentIndex == index ? viewSettings.buttonGreenColor : "white"
                        elide: Text.ElideRight
                        text: modelData
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: pluginMain.valueFontSize
                        font.family: appFont
                        font.styleName: "Regular"

                    }
                }
                onCurrentIndexChanged: selectedDate.setMonth(currentIndex)
            }

            CustomComboBox {
                id: yearBox
                width: root.width * 0.2
                height: pluginMain.buttonHeight
                displayText: currentIndex + firstYear

                model: 50
                itemsVisible: 8
                currentIndex: (selectedDate.getFullYear() >= firstYear) ?
                                  selectedDate.getFullYear() - firstYear : 0

                delegate: ItemDelegate {
                    id: yearDelegate
                    height: yearBox.height
                    width: yearBox.width
                    contentItem: Text {
                        anchors.left: yearDelegate.left
                        anchors.leftMargin: pluginMain.margin
                        text: index + firstYear
                        color: yearBox.currentIndex == index ? viewSettings.buttonGreenColor : "white"
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: pluginMain.valueFontSize
                        font.family: appFont
                        font.styleName: "Regular"
                    }
                }
                onCurrentIndexChanged: selectedDate.setFullYear(currentIndex + firstYear)
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
                displayText: zeroPadTime(currentIndex)

                model: 24
                itemsVisible: 8
                currentIndex: selectedDate.getHours()

                delegate: ItemDelegate {
                    id: hourDelegate
                    height: hourBox.height
                    width: hourBox.width
                    contentItem: Text {
                        anchors.left: hourDelegate.left
                        anchors.leftMargin: pluginMain.margin
                        text: zeroPadTime(parseInt(modelData))
                        color: hourBox.currentIndex == index ? viewSettings.buttonGreenColor : "white"
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: pluginMain.valueFontSize
                        font.family: appFont
                        font.styleName: "Regular"
                    }
                }
                onCurrentIndexChanged: selectedDate.setHours(currentIndex)
            }
            CustomComboBox {
                id: minuteBox
                width: root.width * 0.15
                height: pluginMain.buttonHeight
                displayText: zeroPadTime(currentIndex)

                model: 60
                itemsVisible: 8
                currentIndex: selectedDate.getMinutes()

                delegate: ItemDelegate {
                    id: minuteDelegate
                    height: minuteBox.height
                    width: minuteBox.width
                    contentItem: Text {
                        anchors.left: minuteDelegate.left
                        anchors.leftMargin: pluginMain.margin
                        text: zeroPadTime(parseInt(modelData))
                        color: minuteBox.currentIndex == index ? viewSettings.buttonGreenColor : "white"
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: pluginMain.valueFontSize
                        font.family: appFont
                        font.styleName: "Regular"
                    }
                }
                onCurrentIndexChanged: selectedDate.setMinutes(currentIndex)
            }
        } // Row of time comboboxes

        // Row of set/cancel buttons
        Row {
            spacing: pluginMain.spacing

            QtButton {
                id: dateSetButton
                height: pluginMain.buttonHeight
                text: qsTr("SET")

                onClicked: {
                    selectedDate.setSeconds(0);
                    TimeManager.ntp = false
                    TimeManager.time = selectedDate;
                    console.log("Set date to: " + selectedDate)
                    console.log("TimeManager.time: " + TimeManager.time)
                    settingsLoader.source = "qrc:/timedate/TimeDate.qml"
                }
            }
            QtButton {
                id: dateCancelButton
                height: pluginMain.buttonHeight
                fillColor: viewSettings.buttonGrayColor
                borderColor: "transparent"
                text: qsTr("CANCEL")
                onClicked: {
                    settingsLoader.source = "qrc:/timedate/TimeDate.qml"
                }
            }
        } // Row of set/cancel buttons
    } // Main column
}
