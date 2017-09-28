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
import QtDeviceUtilities.QtButtonImageProvider 1.0

Item {
    id: root
    anchors.fill: parent

    Column {
        spacing: pluginMain.spacing
        anchors.top: parent.top

        Text {
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: pluginMain.subTitleFontSize
            font.family: appFont
            text: qsTr("Brightness ") + (brightnessSlider.value / 255 * 100).toFixed(1) + "%"
            color: "white"
            height: pluginMain.fieldTextHeight
        }

        Slider {
            id: brightnessSlider
            width: root.width
            height: pluginMain.buttonHeight
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true
            from: 0
            to: 255
            onValueChanged: DisplaySettings.setDisplayBrightness(value)

            background: Rectangle {
                id: sliderBackground
                x: brightnessSlider.leftPadding
                y: brightnessSlider.topPadding + brightnessSlider.availableHeight / 2 - height / 2
                implicitWidth: root.width
                implicitHeight: root.height * 0.01
                width: brightnessSlider.availableWidth
                height: implicitHeight
                radius: 2
                color: "#9d9faa"
                Rectangle {
                    width: brightnessSlider.visualPosition * parent.width
                    height: parent.height
                    color: "white"
                    radius: 2
                }
            }
            handle: Rectangle {
                x: brightnessSlider.leftPadding + brightnessSlider.visualPosition * (brightnessSlider.availableWidth - width)
                y: brightnessSlider.topPadding + brightnessSlider.availableHeight / 2 - height / 2
                height: sliderBackground.height * 7
                width: height
                radius: width * 0.5
                antialiasing: true
                color: "#41cd52"
            }

            Component.onCompleted:  brightnessSlider.value = DisplaySettings.displayBrightness
        }

        Text {
            verticalAlignment: Text.AlignVCenter
            height: pluginMain.fieldTextHeight
            font.pixelSize: pluginMain.subTitleFontSize
            font.family: appFont
            text: qsTr("Physical screen size")
            Layout.preferredWidth: root.width * 0.3
            Layout.alignment: Qt.AlignVCenter
            wrapMode: Label.WordWrap
            color: "white"
        }

        Column {
            leftPadding: pluginMain.margin
            spacing: pluginMain.spacing

            Row {
                spacing: pluginMain.spacing

                Text {
                    id: widthText
                    height: pluginMain.buttonHeight
                    text: qsTr("Width")
                    color: "white"
                    font.pixelSize: pluginMain.valueFontSize
                    font.family: appFont
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: widthField
                    height: pluginMain.buttonHeight
                    width: root.width * 0.13
                    color: "black"
                    font.pixelSize: pluginMain.valueFontSize
                    text: DisplaySettings.physicalScreenWidthMm
                    horizontalAlignment: Text.AlignHCenter
                    inputMethodHints: Qt.ImhDigitsOnly
                    background: Rectangle {
                        border.color: widthField.focus ? "#41cd52" : "transparent"
                        border.width: parent.width * 0.05
                    }
                }
                Text {
                    id: heightText
                    height: pluginMain.buttonHeight
                    text: qsTr("Height")
                    color: "white"
                    font.pixelSize: pluginMain.valueFontSize
                    font.family: appFont
                    verticalAlignment: Text.AlignVCenter
                }
                TextField {
                    id: heightField
                    width: widthField.width
                    height: pluginMain.buttonHeight
                    color: "black"
                    font.pixelSize: pluginMain.valueFontSize
                    text: DisplaySettings.physicalScreenHeightMm
                    horizontalAlignment: Text.AlignHCenter
                    inputMethodHints: Qt.ImhDigitsOnly
                    background: Rectangle {
                        border.color: heightField.focus ? "#41cd52" : "transparent"
                        border.width: parent.width * 0.05
                    }
                }
            }

            Row {
                spacing: pluginMain.margin

                QtButton {
                    id: setButton
                    height: pluginMain.buttonHeight
                    fillColor: "#41cd52"
                    borderColor: "transparent"
                    text: qsTr("SET")
                    onClicked: {
                        DisplaySettings.physicalScreenHeightMm = parseInt(heightField.text)
                        DisplaySettings.physicalScreenWidthMm = parseInt(widthField.text)
                    }
                }
                QtButton {
                    id: resetButton
                    height: pluginMain.buttonHeight
                    borderColor: "transparent"
                    fillColor: "#9d9faa"
                    text: qsTr("RESET TO DEFAULT")
                    onClicked: {
                        DisplaySettings.physicalScreenHeightMm = 90
                        DisplaySettings.physicalScreenWidthMm = 154
                    }
                }
            }
        }
    }
}
