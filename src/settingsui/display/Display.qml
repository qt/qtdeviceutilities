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
import QtQuick 2.6
import QtQuick.Layouts 1.3
import Qt.labs.controls 1.0
import Qt.labs.controls.material 1.0
import Qt.labs.controls.universal 1.0
import B2Qt.Utils 1.0

Item {
    id: root
    property string title: qsTr("Display Settings")
    property int titleWidth: width * 0.382

    GroupBox {
        title: qsTr("Display Settings")
        anchors.margins: 20
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right:parent.right

        ColumnLayout {
            anchors.fill: parent

            RowLayout {
                width: parent.width
                spacing: 10

                Label {
                    text: qsTr("Brightness:")
                    Layout.preferredWidth: root.titleWidth
                    Layout.alignment: Qt.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                }
                Slider {
                    id: brightnessSlider
                    value: B2QtDevice.displayBrightness
                    Layout.alignment: Qt.AlignVCenter
                    Layout.fillWidth: true
                    from: 0
                    to: 255
                }
            }
            Binding {
                target: B2QtDevice
                property: "displayBrightness"
                value: brightnessSlider.position * brightnessSlider.to
            }
            GridLayout {
                columns: 2
                rows: 3
                rowSpacing: 10

                Label {
                    text: qsTr("Physical screen size:")
                    Layout.preferredWidth: root.titleWidth
                    Layout.alignment: Qt.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    wrapMode: Label.WordWrap
                }
                RadioButton {
                    text: qsTr("Default")
                    checked: !B2QtDevice.physicalScreenSizeOverride
                }
                RadioButton {
                    id: custom
                    Layout.column: 1
                    Layout.row: 1
                    text: qsTr("Custom")
                    checked: B2QtDevice.physicalScreenSizeOverride
                    onCheckedChanged: B2QtDevice.physicalScreenSizeOverride = checked
                }
                GroupBox {
                    title: qsTr("Size (in inches): %1").arg(B2QtDevice.physicalScreenSizeInch)
                    Layout.column: 1
                    Layout.row: 2
                    Layout.fillWidth: true
                    visible: custom.checked

                    RowLayout {
                        spacing: 10
                        width: parent.width

                        Label {
                            text: sizeSlider.from
                            Layout.alignment: Qt.AlignVCenter
                        }
                        Slider {
                            id: sizeSlider
                            value: B2QtDevice.physicalScreenSizeInch
                            Layout.fillWidth: true
                            stepSize: 1
                            from: 4
                            to: 60
                            onPositionChanged: B2QtDevice.physicalScreenSizeInch = sizeSlider.from + Math.floor(sizeSlider.position * (sizeSlider.to - sizeSlider.from))
                        }
                        Label {
                            text: sizeSlider.to
                            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                        }
                    }
                }
            }
        }
    }
}
