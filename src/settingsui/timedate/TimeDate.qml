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

    Flickable {
        id: flickable
        anchors.fill: parent
        contentHeight: height
        contentWidth: width

        Text {
            id: dateText
            width: parent.width
            height: parent.height * 0.055
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: parent.height * 0.05
            fontSizeMode: Text.Fit
            minimumPixelSize: 1
            font.pixelSize: height
            color: "white"
            font.family: appFont
            Timer {
                id: dateTimer
                interval: 1000
                running: true
                repeat: true
                triggeredOnStart: true
                onTriggered: {
                    var date = new Date();
                    var hours = date.getHours();
                    var minutes = date.getMinutes();
                    var seconds = date.getSeconds();
                    var days = date.getDate();

                    dateText.text = date.toLocaleString(Qt.locale(LocaleManager.locale), Locale.LongFormat)
                }
                function nth(days) {
                    if (days > 3 && days < 21) return 'th';
                    switch (days % 10) {
                    case 1:  return "st";
                    case 2:  return "nd";
                    case 3:  return "rd";
                    default: return "th";
                    }
                }
            }
        }

        Text {
            id: setDateText
            width: parent.width * 0.3
            height: parent.height * 0.05
            anchors.left: parent.left
            anchors.top: dateText.bottom
            anchors.topMargin: parent.height * 0.05
            fontSizeMode: Text.Fit
            minimumPixelSize: 1
            font.pixelSize: height
            color: "white"
            text: qsTr("Set Date & Time")
            font.family: appFont
            font.styleName: "Bold"
        }

        QtButton {
            id: automaticButton
            anchors.top: setDateText.bottom
            anchors.topMargin: parent.height * 0.025
            anchors.left: parent.left
            height: parent.height * 0.075
            fillColor: "#41cd52"
            borderColor: "transparent"
            text: qsTr("AUTOMATICALLY")
            onClicked: TimeManager.ntp = true
        }

        QtButton {
            id: manualButton
            anchors.top: setDateText.bottom
            anchors.topMargin: parent.height * 0.025
            anchors.left: automaticButton.right
            anchors.leftMargin: root.margin * 0.5
            height: parent.height * 0.075
            fillColor: "#9d9faa"
            borderColor: "transparent"
            text: qsTr("MANUALLY")
            onClicked: settingsLoader.source = "qrc:/timedate/ManualTime.qml"
        }

        Text {
            id: timeZoneText
            width: parent.width * 0.3
            height: parent.height * 0.05
            anchors.left: parent.left
            anchors.top: automaticButton.bottom
            anchors.topMargin: parent.height * 0.05
            fontSizeMode: Text.Fit
            minimumPixelSize: 1
            font.pixelSize: height
            color: "white"
            text: qsTr("Time Zone")
            font.family: appFont
            font.styleName: "Bold"
        }

        CustomComboBox {
            id: timeZoneBox
            width: automaticButton.width + manualButton.width + root.margin * 0.5
            height: parent.height * 0.075
            anchors.top: timeZoneText.bottom
            anchors.topMargin: parent.height * 0.025
            anchors.left: parent.left
            textRole: "id"
            model: TimezonesFilter
            delegate: ItemDelegate {
                id: timeZoneDelegate
                background: Item {}
                width: timeZoneBox.width
                contentItem: Text {
                    anchors.left: timeZoneDelegate.left
                    anchors.leftMargin: timeZoneDelegate.width * 0.05
                    text: modelData["id"]
                    font.family: appFont
                    font.styleName: timeZoneBox.currentIndex == index ? "Bold" : "Regular"
                    color: timeZoneBox.currentIndex == index ? "#41cd52" : "white"
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Component.onCompleted: {
                var n = TimezonesFilter.indexForTimezone(TimeManager.timeZone)
                timeZoneBox.currentIndex = n
            }

            Connections {
                target: TimezonesFilter.sourceModel
                onReady: {
                    var n = TimezonesFilter.indexForTimezone(TimeManager.timeZone)
                    timeZoneBox.currentIndex = n
                }
            }


            onCurrentIndexChanged: {
                var val = TimezonesFilter.itemFromRow(currentIndex);
                if (val !== "") {
                    TimeManager.timeZone = val
                }
            }
            contentItem: Text {
                anchors.left: timeZoneBox.left
                anchors.leftMargin: timeZoneBox.width * 0.05
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: timeZoneBox.height * 0.35
                font.bold: true
                color: "white"
                elide: Text.ElideRight
                text: timeZoneBox.displayText
                font.family: appFont
                font.styleName: "SemiBold"
            }
        }

        Component {
            id: zoneselect
            TimezonesView {}
        }
    }
}
