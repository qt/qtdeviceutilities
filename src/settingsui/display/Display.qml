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
import QtDeviceUtilities.DisplaySettings 1.0

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
                    value: DisplaySettings.displayBrightness
                    Layout.alignment: Qt.AlignVCenter
                    Layout.fillWidth: true
                    from: 0
                    to: 255
                }
            }
            Binding {
                target: DisplaySettings
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
                    checked: !DisplaySettings.physicalScreenSizeOverride
                }
                RadioButton {
                    id: custom
                    Layout.column: 1
                    Layout.row: 1
                    text: qsTr("Custom")
                    checked: DisplaySettings.physicalScreenSizeOverride
                    onCheckedChanged: DisplaySettings.physicalScreenSizeOverride = checked
                }
                GroupBox {
                    title: qsTr("Size (in inches): %1").arg(DisplaySettings.physicalScreenSizeInch)
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
                            value: DisplaySettings.physicalScreenSizeInch
                            Layout.fillWidth: true
                            stepSize: 1
                            from: 4
                            to: 60
                            onPositionChanged: DisplaySettings.physicalScreenSizeInch = sizeSlider.from + Math.floor(sizeSlider.position * (sizeSlider.to - sizeSlider.from))
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
