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
import QtDeviceUtilities.LocaleSettings 1.0
import QtQml 2.2
import QtDeviceUtilities.QtButtonImageProvider 1.0

Item {
    id: root
    property var currentRegion: Qt.locale(LocaleManager.locale)

    Column {
        spacing: pluginMain.spacing

        Text {
            color: "white"
            text: qsTr("Region")
            font.pixelSize: pluginMain.subTitleFontSize
            font.family: appFont
        }

        Row {
            leftPadding: pluginMain.margin
            spacing: pluginMain.spacing
            Text {
                color: "white"
                text: root.currentRegion.nativeCountryName == "" ? "Default" : root.currentRegion.nativeCountryName
                font.pixelSize: pluginMain.valueFontSize
                font.family: appFont
                verticalAlignment: Text.AlignVCenter
                height: parent.height
            }
            QtButton {
                id: regionButton
                height: pluginMain.buttonHeight
                fillColor: "#41cd52"
                text: qsTr("CHANGE")
                onClicked: settingsLoader.source = "qrc:/locale/RegionSelect.qml"
            }
        }

        Text {
            color: "white"
            text: qsTr("Format")
            font.pixelSize: pluginMain.subTitleFontSize
            font.family: appFont
        }

        Grid {
            columns: 2
            leftPadding: pluginMain.margin
            spacing: pluginMain.spacing

            TableKey {
                text: qsTr("Region")
            }
            TableValue {
                text: root.currentRegion.nativeCountryName
            }

            TableKey {
                text: qsTr("Short date:")
            }
            TableValue {
                text: root.currentRegion.dateFormat(1)
            }
            TableKey {
                text: qsTr("Short time:")
            }
            TableValue {
                text: root.currentRegion.timeFormat(1)
            }
            TableKey {
                text: qsTr("Currency:")
            }
            TableValue {
                text: root.currentRegion.currencySymbol(1)
            }
            TableKey {
                text: qsTr("First day of week:")
            }
            TableValue {
                text: root.currentRegion.firstDayOfWeek
            }
        }
    }
}
