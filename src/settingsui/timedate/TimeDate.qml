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

Item {
    id: root
    property string title: qsTr("Time and Date settings")

    Flickable {
        id: flickable
        anchors.margins: 20
        anchors.fill: parent
        contentHeight: content.height
        contentWidth: width
        interactive: !clock.handPressed

        ColumnLayout {
            id: content
            spacing: 20
            width: parent.width

            GroupBox {
                width: parent.width
                title: qsTr("Time Settings")
                Layout.fillWidth: true

                ColumnLayout {
                    spacing: 10
                    width: parent.width
                    Layout.fillWidth: true

                    RadioButton {
                        id: automatic
                        text: qsTr("Automatic time set")
                        checked: TimeManager.ntp
                        onCheckedChanged: if (checked) updateTimer.restart()

                        Timer {
                            id: updateTimer
                            interval: 300
                            onTriggered: calendar.updateDate()
                        }
                    }
                    RadioButton {
                        id: custom
                        text: qsTr("Manual")
                        checked: !TimeManager.ntp
                        onCheckedChanged: TimeManager.ntp = !checked
                    }
                    RowLayout
                    {
                        id: layout
                        spacing: 10

                        CustomCalendar {
                            id: calendar
                            width: height
                        }
                        AnalogClock {
                            id: clock
                            height: calendar.height
                            width: height
                            editMode: !automatic.checked
                        }
                    }
                    Component {
                        id: zoneselect
                        TimezonesView { }
                    }
                }
            }
            GroupBox {
                title: qsTr("Timezone Settings")
                Layout.fillWidth: true
                width: parent.width
                visible: true

                RowLayout {
                    spacing: 10
                    Label {
                        text: TimeManager.timeZone
                        Layout.alignment: Qt.AlignVCenter
                    }
                    Button {
                        text: qsTr("Change")
                        onClicked : stackView.push(zoneselect)
                    }
                }
            }
        }
    }
}
