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
import B2Qt.Utils 1.0
import QtQuick.Enterprise.VirtualKeyboard.Settings 1.2

Item {
    id: root

    property string title: qsTr("Display Settings")

    GroupBox {
        title: qsTr("Display Settings")
        anchors.fill: parent
        anchors.margins: Math.round(40 * Flat.FlatStyle.scaleFactor)
        Layout.fillWidth: true

        Column {
            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
            RowLayout {
                spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)

                TextLabel {
                    text: qsTr("Brighness: ")
                }

                Slider {
                    id: brightnessSlider
                    value: B2QtDevice.displayBrightness
                    Layout.fillWidth: true
                }
            }

            Binding {
                target: B2QtDevice
                property: "displayBrightness"
                value: brightnessSlider.value
            }

            Row {
                spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)

                TextLabel {
                    text: qsTr("Physical screen size: ")
                }

                ColumnLayout {
                    ExclusiveGroup { id: exgroup }
                    RadioButton {
                        text: qsTr("Default")
                        exclusiveGroup: exgroup
                        checked: !B2QtDevice.physicalScreenSizeOverride
                    }
                    RadioButton {
                        id: custom
                        text: qsTr("Custom")
                        exclusiveGroup: exgroup
                        checked: B2QtDevice.physicalScreenSizeOverride

                        onCheckedChanged: B2QtDevice.physicalScreenSizeOverride = checked
                   }

                   GroupBox {
                        title: qsTr("Size (in inches): " + sizeSlider.value)
                        Layout.fillWidth: true
                        flat: true
                        visible: custom.checked

                        Row {
                            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
                            TextLabel {
                                text: sizeSlider.minimumValue
                            }

                            Slider {
                                id: sizeSlider
                                value: B2QtDevice.physicalScreenSizeInch
                                Layout.fillWidth: true
                                stepSize: 1
                                minimumValue: 4
                                maximumValue: 60
                            }

                            TextLabel {
                                text: sizeSlider.maximumValue
                            }

                            Binding {
                                target: B2QtDevice
                                property: "physicalScreenSizeInch"
                                value: sizeSlider.value
                            }
                        }
                    }
                }
            }
        }
    }
}

