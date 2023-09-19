/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
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
