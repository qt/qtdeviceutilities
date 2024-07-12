// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import DeviceUtilities.SettingsUI

ColumnLayout {
    id: root

    Layout.fillHeight: true
    Layout.fillWidth: true

    property var headerTexts: []
    property var roleNames: []
    property alias model: listView.model
    property alias localeIndex: listView.currentIndex
    signal clicked(int index)

    Timer { //A workaround to position the listView..
        interval: 250
        running: true
        onTriggered: listView.positionViewAtIndex(listView.currentIndex, ListView.Beginning)
    }

    ListView {
        id: listView
        Layout.fillHeight: true
        Layout.fillWidth: true
        clip: true

        onCurrentIndexChanged: {
            if (listView.model) {
                positionViewAtIndex(listView.currentIndex, ListView.beginning)
            }
        }

        ScrollBar.vertical: ScrollBar {
            contentItem: Rectangle {
                implicitWidth: listView.width * 0.0175
                implicitHeight: listView.height * 0.5
                color: Globals.scrollBarColor
            }
        }

        delegate: Item {
            id: delegateRectangle
            width: parent.width
            height: listView.height * 0.1
            property var delegateData: entry

            Row {
                anchors.fill: parent
                anchors.leftMargin: 10
                spacing: 10

                Repeater {
                    model: root.roleNames.length

                    Text {
                        width: parent.width / root.roleNames.length
                        text: delegateData[root.roleNames[index]]
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: delegateRectangle.height * 0.5
                        antialiasing: false
                        smooth: false
                        renderType: listView.moving ? Text.NativeRendering : Text.QtRendering
                        color: listView.currentIndex == delegateRectangle.index ? Globals.buttonGreenColor : "white"
                        font.family: Globals.appFont
                    }
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: root.clicked(index)
            }
        }
    }
}
