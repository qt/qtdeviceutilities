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
import com.theqtcompany.settings.common 1.0
import com.theqtcompany.settings.timedate 1.0

Item {
    property var currentTime: TimeManager.time
    property var newTime: new Date
    property bool editMode: false

    function showTime(time) {
        return time.getHours() + ":" + time.getMinutes()
    }

    onEditModeChanged: if (editMode) newTime = new Date

    Rectangle {
        border.color: "#bdbebf"
        border.width: 1
        color: "white"
        anchors.fill: parent

        Connections {
            target: TimeManager
            onTimeChanged : newTime.setSeconds(currentTime.getSeconds())
            onTimeZoneChanged : Date.timeZoneUpdated()
        }

        TextLabel {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 10
            anchors.top: parent.top
            text: editMode ? newTime.toTimeString() : currentTime.toTimeString()
        }

        Rectangle {
            id: root
            anchors.fill: parent
            anchors.margins: Math.round(40 * Flat.FlatStyle.scaleFactor)
            color: "white"
            border.color: editMode ? "#d6d6d6" : "#5caa15"
            border.width: Math.round(root.width * 0.120)
            radius: parent.width / 2
            property int handOffset: Math.round(root.width * 0.040)
            antialiasing: true

            Rectangle {
                id: minutes
                x: root.height / 2 - width / 2
                y: root.height / 2 - height + root.handOffset
                color: editMode ? "#d6d6d6" : "#5caa15"
                width: Math.round(root.width * 0.080)
                height: Math.round(root.height / 2 * 0.65 + root.handOffset)
                antialiasing: true
                transform: Rotation {
                    id: minuteRotation
                    origin.x: Math.round(minutes.width / 2)
                    origin.y: Math.round(minutes.height - root.handOffset)
                    angle: currentTime.getMinutes() * 6
                    Behavior on angle {
                        SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
                    }
                }
            }

            Rectangle {
                id: hours
                x: Math.round(root.height / 2 - width / 2)
                y: Math.round(root.height / 2 - height + root.handOffset)
                height: Math.round(root.height / 2 * 0.4 + root.handOffset)
                width: Math.round(root.width * 0.080)
                color: editMode ? "#d6d6d6" : "#80c342"
                antialiasing: true
                transform: Rotation {
                    id: hourRotation
                    origin.x: Math.round(hours.width / 2)
                    origin.y: hours.height - root.handOffset;
                    angle: (currentTime.getHours() * 30) + (currentTime.getMinutes() * 0.5)
                    Behavior on angle {
                        SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
                    }
                }
            }

            Rectangle {
                id: seconds
                x: root.height / 2 - width / 2
                y: root.height / 2 - height + root.handOffset
                visible: !editMode
                color: "#46a2da"
                width: Math.round(root.width * 0.0128)
                height: Math.round(root.height / 2 * 0.74)
                radius: Math.round(width / 2)
                antialiasing: true
                transform: Rotation {
                    id: secondRotation
                    origin.x: Math.round(seconds.width / 2)
                    origin.y: Math.round(seconds.height - root.handOffset)
                    angle: currentTime.getSeconds() * 6
                    Behavior on angle {
                        SpringAnimation { spring: 2; damping: 0.2; modulus: 360 }
                    }
                }
            }

            Component {
                 id: editor

                 Rectangle {
                     id: rect
                     property var angle: mouseArea.drag ? mouseArea.angle : defaultAngle
                     property var defaultAngle: angleBinding
                     x: Math.round(Math.cos((-90+ angle)*Math.PI/180) *
                        (pos - root.handOffset - width /2 + radius) + root.width / 2 - width / 2)
                     y: Math.round(Math.sin((-90+ angle)*Math.PI/180) *
                        (pos - root.handOffset - width /2 + radius) + root.height / 2 - width / 2)
                     color: "#5caa15"
                     width: size
                     radius: width / 2
                     height: width
                     antialiasing: true

                     function calcAngle(mouse) {
                         var mouseGlobal = mapToItem(root, mouse.x, mouse.y)
                         var origin = root.width/2
                         var angle = (90+Math.atan2((mouseGlobal.y-origin), (mouseGlobal.x-origin))*180/Math.PI)
                         if (angle < 0)
                             angle += 360;
                         updateAngle(angle);
                         return angle;
                     }

                     MouseArea {
                         id: mouseArea
                         anchors.fill: parent
                         property int startX: 0
                         property int startY: 0
                         property bool drag: false
                         property var angle: 0.0
                         preventStealing: true

                         onPressed: {
                             var mouseGlobal = mapToItem(root, mouse.x, mouse.y);
                             startX = mouseGlobal.x;
                             startY = mouseGlobal.y;
                             angle = calcAngle(mouse);
                             drag = true;
                         }
                         onReleased: {
                             drag = false;
                             ready(angle)
                         }
                         onMouseXChanged: if (drag) angle = calcAngle(mouse)
                     }
                 }
             }

            Loader {
                property var angleBinding: (currentTime.getHours() * 30) + (currentTime.getMinutes() * 0.5)
                property int size: Math.round(root.width * 0.120)
                property int pos: hours.height
                property bool pm: false
                visible: editMode

                function updateAngle(angle) {
                    var newHour = Math.floor(angle / 30);
                    var preHour = newTime.getHours();

                    if (preHour == 11 && newHour == 0) {
                        newHour = 12;
                        pm = true;
                    }
                    else if (preHour === 23 && newHour === 0) {
                        pm = false;
                    }
                    else if (preHour == 0 && newHour === 11) {
                        pm = true;
                    }
                    else if (preHour == 12 && newHour == 11) {
                        pm = false;
                    }

                    if (pm == true) {
                        newHour += 12;
                    }

                    newTime.setHours(newHour);
                }

                function ready(val) {
                    var newhours = Math.round(val / 30);
                    currentTime.setHours(newhours);
                    TimeManager.time = currentTime;
                }
                sourceComponent: editor
            }

            Loader {
                property var angleBinding: currentTime.getMinutes() * 6
                property int size: Math.round(root.width * 0.120)
                property int pos: minutes.height
                visible: editMode
                sourceComponent: editor

                function updateAngle(angle) {
                    var newMin = Math.round(angle / 6);
                    var hours = newTime.getHours();
                    newTime.setMinutes(newMin);
                    newTime.setHours(hours);
                }

                function ready(val) {
                    var newmins = Math.round(val / 6);
                    currentTime.setMinutes(newmins);
                    TimeManager.time = currentTime;
                }
            }
        }
    }
}

