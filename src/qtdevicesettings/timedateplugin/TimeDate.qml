/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles.Flat 1.0 as Flat
import com.theqtcompany.settings.common 1.0
import com.theqtcompany.settings.timedate 1.0

Item {
    id: root
    property string title: qsTr("Time and Date settings")

    Flickable {
        anchors.fill: parent
        anchors.margins: Math.round(40 * Flat.FlatStyle.scaleFactor)
        anchors.bottomMargin: Math.round(20 * Flat.FlatStyle.scaleFactor)
        contentHeight: content.height
        contentWidth: width

        Column {
            id: content
            width: parent.width

            GroupBox {
                width: parent.width
                title: qsTr("Time Settings")
                Layout.fillWidth: true

                ColumnLayout {
                    spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                    width: parent.width
                    Layout.fillWidth: true

                    ExclusiveGroup { id: exgroup }

                    RadioButton {
                        id: automatic
                        text: qsTr("Automatic time set")
                        exclusiveGroup: exgroup
                        checked: TimeManager.ntp
                        onCheckedChanged: if (checked) calloader.reload();
                    }
                    RadioButton {
                        id: custom
                        text: qsTr("Manual")
                        exclusiveGroup: exgroup
                        checked: !TimeManager.ntp
                        onCheckedChanged: TimeManager.ntp = !checked
                   }
                   RowLayout {
                       spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                       Layout.fillWidth: true

                       Component {
                           id: calendar

                           Item {
                               width: cal.width
                               height: cal.height

                               Timer {
                                   id: timer
                               }
                               function delay(delayTime, cb) {
                                   timer.interval = delayTime;
                                   timer.repeat = false;
                                   timer.triggered.connect(cb);
                                   timer.start();
                               }

                               Calendar {
                                   id: cal
                                   weekNumbersVisible: false
                                   enabled: !automatic.checked
                                   onClicked: {
                                       var currentTime = TimeManager.time;
                                       var newDate = date;
                                       newDate.setHours(currentTime.getHours());
                                       newDate.setMinutes(currentTime.getMinutes());
                                       newDate.setSeconds(currentTime.getSeconds());
                                       TimeManager.time = newDate;
                                       delay(100, function() {
                                           calloader.reload();
                                       });
                                   }
                               }
                           }
                       }

                       Loader {
                           id: calloader
                           sourceComponent: calendar
                           function reload() {
                               calloader.sourceComponent = undefined;
                               calloader.sourceComponent = calendar;
                           }
                       }

                       AnalogClock {
                           id: clock
                           width: calloader.width
                           height: width
                           editMode: !automatic.checked
                       }
                   }

                   Component {
                       id: zoneselect
                       TimezonesView { }
                   }

                   GroupBox {
                       title: qsTr("Timezone Settings")
                       Layout.fillWidth: true
                       width: parent.width
                       visible: true
                       flat: true
                       Row {
                           spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                           TextLabel {
                               text: TimeManager.timeZone
                           }
                           Button {
                               text: "Change"
                               onClicked : stackView.push(zoneselect)
                           }
                       }
                   }
                }
            }
        }
    }
}
