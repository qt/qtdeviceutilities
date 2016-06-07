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
import Qt.labs.calendar 1.0
import QtQml 2.2
import QtDeviceUtilities.TimeDateSettings 1.0

Rectangle {
    id: root
    border.color: "#bdbebf"
    border.width: 1
    color: "white"
    height: cal.height

    function updateDate() {
        var date = new Date()
        grid.month = date.getMonth()
        grid.date = date.getDate()
        grid.year = date.getFullYear()
    }

    ColumnLayout {
        id: cal
        width: root.width
        spacing: 10
        enabled: !automatic.checked

        RowLayout {
            spacing: 0
            Layout.alignment: Qt.AlignTop

            Button {
                id: previousMonth
                Layout.preferredWidth: height
                visible: enabled
                contentItem: Rectangle {
                    anchors.fill: parent
                    color: "#d6d6d6"

                    Image {
                        anchors.centerIn: parent
                        source: "../icons/Chevron-left_black_1x.png"
                    }
                }
                onClicked: {
                    if (grid.month === Calendar.January) {
                        grid.year--
                        grid.month = Calendar.December
                        return;
                    }
                    grid.month--
                }
            }
            Rectangle {
                Layout.fillWidth: true
                color: enabled ? "#d6d6d6" : "#80c342"
                height: previousMonth.height

                Label {
                    id: title
                    text: Qt.locale().monthName(grid.month, Locale.LongFormat) + " " + grid.year
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
            Button {
                id: nextMonth
                Layout.preferredWidth: height
                visible: enabled
                contentItem: Rectangle {
                    anchors.fill: parent
                    color: "#d6d6d6"

                    Image {
                        anchors.centerIn: parent
                        source: "../icons/Chevron-left_black_1x.png"
                        mirror: true
                    }
                }
                onClicked: {
                    if (grid.month === Calendar.December) {
                        grid.year++
                        grid.month = Calendar.January
                    }
                    grid.month++
                }
            }
        }
        DayOfWeekRow {
            locale: grid.locale
            Layout.column: 1
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            delegate: Text {
                text: model.narrowName.charAt(0)
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        MonthGrid {
            id: grid
            Layout.alignment: Qt.AlignTop
            Layout.fillWidth: true
            property int date: -1
            property Item currentItem: null
            delegate: Label {
                id: gridDelegate
                objectName: "gridDelegate"
                text: delegateDay
                opacity: model.month === grid.month ? 1 : .2
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: delegateDay === grid.date && delegateMonth === grid.month
                color: delegateDay === grid.date && delegateMonth === grid.month && enabled ? "white" : "black"
                property int delegateDay: model.day
                property int delegateMonth: model.month
            }
            Component.onCompleted: updateDate()

            Rectangle {
                z: -1
                id: highlight
                x: grid.currentItem ? grid.currentItem.x + grid.currentItem.width / 2 - width / 2: 0
                y: grid.currentItem ? grid.currentItem.y + grid.currentItem.height / 2- height / 2: 0
                width: grid.currentItem ? grid.currentItem.width : 0
                visible: grid.currentItem
                height: width
                color: enabled? "#80c342" : "#d6d6d6"
                radius: width / 2
            }

            onMonthChanged: updateHighlightPosition()
            onYearChanged: updateHighlightPosition()
            onDateChanged: updateHighlightPosition()

            function updateHighlightPosition() {
                var date = new Date()

                date.setFullYear(grid.year)
                date.setMonth(grid.month)
                date.setDate(grid.date)

                var index = grid.source.indexOf(date)
                var delegate = grid.contentItem.children[index]

                if (delegate)
                    grid.currentItem = delegate
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    var item = grid.contentItem.childAt(mouse.x, mouse.y)

                    if (item) {
                        if (item.objectName !== "gridDelegate")
                            item = grid.contentItem.children[0]

                        if (!item)
                            return;

                        grid.currentItem = item
                        grid.date = item.delegateDay
                        grid.month = item.delegateMonth

                        var currentTime = TimeManager.time;
                        var newDate = new Date();

                        newDate.setFullYear(grid.year)
                        newDate.setMonth(grid.month)
                        newDate.setDate(grid.date)
                        newDate.setHours(currentTime.getHours());
                        newDate.setMinutes(currentTime.getMinutes());
                        newDate.setSeconds(currentTime.getSeconds());
                        TimeManager.time = newDate;
                    }
                }
            }
        }
    }
}
