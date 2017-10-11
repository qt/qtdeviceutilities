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
import QtDemoLauncher.QtButtonImageProvider 1.0

Item {
    id: root
    property string title: qsTr("Display Settings")
    property int titleWidth: width * 0.382
    property int margin: root.width * 0.05

    Text {
        id: displayText
        anchors.top: parent.top
        anchors.left: parent.left
        font.pixelSize: parent.height * 0.045
        color: "white"
        text: qsTr("Display")
    }

    Rectangle {
        id: btmLine
        anchors.top: displayText.bottom
        anchors.topMargin: parent.height * 0.025
        anchors.left: displayText.left
        width: parent.width * 0.275
        height: parent.height * 0.005
    }

    Label {
        id: brightnessLabel
        anchors.top: btmLine.bottom
        anchors.topMargin: parent.height * 0.1
        font.pixelSize: parent.height * 0.04
        text: qsTr("Brightness ") + (brightnessSlider.valueAt(brightnessSlider.position) / 255 * 100).toFixed(1) + "%"
        horizontalAlignment: Text.AlignRight
        color: "white"
    }

    Slider {
        id: brightnessSlider
        anchors.top: brightnessLabel.bottom
        anchors.topMargin: parent.height * 0.05
        anchors.left: parent.left
        anchors.right: parent.right
        value: DisplaySettings.displayBrightness
        Layout.alignment: Qt.AlignVCenter
        Layout.fillWidth: true
        from: 0
        to: 255
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
            radius: 1000
            antialiasing: true
            color: "#41cd52"
        }
    }

    Label {
        id: screenSizeLabel
        anchors.top: brightnessSlider.bottom
        anchors.topMargin: parent.height * 0.1
        font.pixelSize: parent.height * 0.04
        text: qsTr("Physical screen size")
        Layout.preferredWidth: root.titleWidth
        Layout.alignment: Qt.AlignVCenter
        horizontalAlignment: Text.AlignRight
        wrapMode: Label.WordWrap
        color: "white"
    }

    Row {
        id: widthRow
        height: parent.height * 0.075
        anchors.top: screenSizeLabel.bottom
        anchors.topMargin: parent.height * 0.035
        anchors.left: parent.left
        Text {
            id: widthText
            height: parent.height
            width: root.width * 0.075
            text: qsTr("Width")
            color: "white"
            font.pixelSize: height * 0.35
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }
        TextField {
            id: widthField
            width: root.width * 0.1
            height: parent.height
            color: "black"
            text: DisplaySettings.physicalScreenWidthMm
            inputMethodHints: Qt.ImhDigitsOnly
            background: Rectangle {
                border.color: widthField.focus ? "#41cd52" : "transparent"
                border.width: parent.width * 0.05
            }
        }
    }

    Row {
        id: heightRow
        height: parent.height * 0.075
        anchors.top: screenSizeLabel.bottom
        anchors.topMargin: parent.height * 0.035
        anchors.left: widthRow.right
        anchors.leftMargin: root.margin * 0.5
        Text {
            id: heightText
            height: parent.height
            width: root.width * 0.085
            text: qsTr("Height")
            color: "white"
            font.pixelSize: height * 0.35
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }
        TextField {
            id: heightField
            width: root.width * 0.1
            height: parent.height
            color: "black"
            text: DisplaySettings.physicalScreenHeightMm
            inputMethodHints: Qt.ImhDigitsOnly
            background: Rectangle {
                border.color: heightField.focus ? "#41cd52" : "transparent"
                border.width: parent.width * 0.05
            }
        }
    }

    QtButton {
        id: setButton
        height: parent.height * 0.075
        anchors.top: heightRow.bottom
        anchors.topMargin: parent.height * 0.035
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
        height: parent.height * 0.075
        anchors.top: heightRow.bottom
        anchors.topMargin: parent.height * 0.035
        anchors.left: setButton.right
        anchors.leftMargin: root.margin * 0.5
        borderColor: "transparent"
        fillColor: "#9d9faa"
        text: qsTr("RESET TO DEFAULT")
        onClicked: {
            DisplaySettings.physicalScreenHeightMm = 90
            DisplaySettings.physicalScreenWidthMm = 154
        }
    }
}
