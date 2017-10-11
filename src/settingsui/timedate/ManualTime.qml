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
import QtQuick 2.0
import QtQuick.Controls 2.0
import QtDemoLauncher.QtButtonImageProvider 1.0
import QtDeviceUtilities.TimeDateSettings 1.0

Item {
    id: root
    property int margin: root.width * 0.05

    property int firstYear: 2017

    function zeroPadTime(timeToPad) {
        return timeToPad < 10 ? "0" + timeToPad : timeToPad
    }

    Text {
        id: setDateAndTimeText
        anchors.top: parent.top
        anchors.left: parent.left
        fontSizeMode: Text.Fit
        minimumPixelSize: 1
        font.pixelSize: parent.height * 0.05
        color: "white"
        text: qsTr("Set Date & Time")
        font.family: appFont
        font.styleName: "Bold"
    }
    Rectangle {
        id: btmLine
        anchors.top: setDateAndTimeText.bottom
        anchors.topMargin: parent.height * 0.025
        anchors.left: setDateAndTimeText.left
        width: parent.width * 0.275
        height: parent.height * 0.005
    }

    Text {
        id: setDateText
        anchors.top: btmLine.bottom
        anchors.topMargin: parent.height * 0.075
        anchors.left: parent.left
        color: "white"
        font.pixelSize: parent.height * 0.04
        text: qsTr("Set Date")
        font.family: appFont
        font.styleName: "Bold"
    }

    CustomComboBox {
        id: dayBox
        width: parent.width * 0.15
        height: parent.height * 0.075
        anchors.left: parent.left
        anchors.top: setDateText.bottom
        anchors.topMargin: parent.height * 0.025
        displayText: currentIndex + 1
        property var date: new Date(firstYear, 1, 0)

        model: date.getDate()

        delegate: ItemDelegate {
            id: dayDelegate
            background: Item {}
            width: dayBox.width
            contentItem: Text {
                anchors.left: dayDelegate.left
                anchors.leftMargin: dayDelegate.width * 0.15
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: dayBox.height * 0.35
                color: dayBox.currentIndex == index ? "#41cd52" : "white"
                text: modelData + 1
                font.family: appFont
                font.styleName: dayBox.currentIndex == index ? "Bold" : "Regular"
                elide: Text.ElideRight
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
        width: parent.width * 0.35
        height: parent.height * 0.075
        anchors.top: setDateText.bottom
        anchors.topMargin: parent.height * 0.025
        anchors.left: dayBox.right
        anchors.leftMargin: root.margin * 0.5

        model: ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"]
    }
    CustomComboBox {
        id: yearBox
        width: parent.width * 0.2
        height: parent.height * 0.075
        anchors.top: setDateText.bottom
        anchors.topMargin: parent.height * 0.025
        anchors.left: monthBox.right
        anchors.leftMargin: root.margin * 0.5

        model: 50
        delegate: ItemDelegate {
            id: yearDelegate
            background: Item {}
            width: yearBox.width
            contentItem: Text {
                anchors.left: yearDelegate.left
                anchors.leftMargin: yearDelegate.width * 0.15
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: yearBox.height * 0.35
                text: index + firstYear
                color: yearBox.currentIndex == index ? "#41cd52" : "white"
                font.family: appFont
                font.styleName: yearBox.currentIndex == index ? "Bold" : "Regular"
                elide: Text.ElideRight

            }
        }
        contentItem: Text {
            property var date: new Date()
            anchors.left: yearBox.left
            anchors.leftMargin: yearBox.width * 0.15
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: yearBox.height * 0.35
            color: "white"
            elide: Text.ElideRight
            text: yearBox.currentIndex + date.getFullYear()
            font.family: appFont
            font.styleName: "SemiBold"
        }
    }

    Text {
        id: setTimeText
        anchors.top: dayBox.bottom
        anchors.topMargin: parent.height * 0.075
        anchors.left: parent.left
        color: "white"
        font.pixelSize: parent.height * 0.04
        text: qsTr("Set Time")
        font.family: appFont
        font.styleName: "Bold"
    }

    CustomComboBox {
        id: hourBox
        width: parent.width * 0.15
        height: parent.height * 0.075
        anchors.top: setTimeText.bottom
        anchors.topMargin: parent.height * 0.025
        anchors.left: parent.left
        property int hour: currentIndex

        model: 24

        delegate: ItemDelegate {
            id: hourDelegate
            background: Item {}
            width: hourBox.width
            contentItem: Text {
                anchors.left: hourDelegate.left
                anchors.leftMargin: hourDelegate.width * 0.15
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: hourBox.height * 0.35
                color: hourBox.currentIndex == index ? "#41cd52" : "white"
                text: zeroPadTime(parseInt(modelData))
                elide: Text.ElideRight
                font.family: appFont
                font.styleName: hourBox.currentIndex == index ? "Bold" : "Regular"
            }
        }
        contentItem: Text {
            anchors.left: hourBox.left
            anchors.leftMargin: hourBox.width * 0.15
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: hourBox.height * 0.35
            font.bold: true
            color: "white"
            elide: Text.ElideRight
            text: zeroPadTime(hourBox.displayText)
            font.family: appFont
            font.styleName: "SemiBold"
        }
    }
    CustomComboBox {
        id: minuteBox
        width: parent.width * 0.15
        height: parent.height * 0.075
        anchors.top: setTimeText.bottom
        anchors.topMargin: parent.height * 0.025
        anchors.left: hourBox.right
        anchors.leftMargin: root.margin * 0.5
        property int minute: currentIndex

        model: 60

        delegate: ItemDelegate {
            id: minuteDelegate
            background: Item {}
            width: minuteBox.width
            contentItem: Text {
                anchors.left: minuteDelegate.left
                anchors.leftMargin: minuteDelegate.width * 0.15
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: minuteBox.height * 0.35
                color: minuteBox.currentIndex == index ? "#41cd52" : "white"
                text: zeroPadTime(parseInt(modelData))
                font.family: appFont
                font.styleName: minuteBox.currentIndex == index ? "Bold" : "Regular"
                elide: Text.ElideRight
            }
        }
        contentItem: Text {
            anchors.left: minuteBox.left
            anchors.leftMargin: minuteBox.width * 0.15
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: minuteBox.height * 0.35
            font.family: appFont
            font.styleName: "SemiBold"
            color: "white"
            elide: Text.ElideRight
            text: zeroPadTime(parseInt(minuteBox.displayText))
        }
    }

    QtButton {
        id: dateSetButton
        height: parent.height * 0.075
        anchors.top: hourBox.bottom
        anchors.topMargin: parent.height * 0.1
        anchors.left: parent.left
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
        }
    }
    QtButton {
        id: dateCancelButton
        height: parent.height * 0.075
        anchors.top: hourBox.bottom
        anchors.topMargin: parent.height * 0.1
        anchors.left: dateSetButton.right
        anchors.leftMargin: root.margin * 0.5
        fillColor: "#9d9faa"
        borderColor: "transparent"
        text: qsTr("CANCEL")
        onClicked: settingsLoader.source = "qrc:/timedate/TimeDate.qml"
    }
}
