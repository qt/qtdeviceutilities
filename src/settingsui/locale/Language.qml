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
import QtDemoLauncher.QtButtonImageProvider 1.0

Item {
    id: root
    property var currentRegion: Qt.locale(LocaleManager.locale)

    Row {
        id: regionRow
        anchors.top: parent.top
        anchors.topMargin: parent.height * 0.075
        anchors.left: parent.left
        height: parent.height * 0.075

        Text {
            width: root.width * 0.2
            height: root.height * 0.04
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: height
            color: "white"
            text: qsTr("Region")
            font.family: appFont
            font.styleName: "Bold"
        }
        QtButton {
            id: regionButton
            height: parent.height
            fillColor: "#41cd52"
            text: root.currentRegion.nativeCountryName
            onClicked: settingsLoader.source = "qrc:/locale/RegionSelect.qml"
        }
    }

    Text {
        id: formatText
        width: root.width * 0.2
        anchors.top: regionRow.bottom
        anchors.topMargin: parent.height * 0.1
        anchors.left: parent.left
        font.pixelSize: parent.height * 0.04
        text: qsTr("Format")
        horizontalAlignment: Text.AlignLeft
        color: "white"
        font.family: appFont
        font.styleName: "Bold"
    }

    Column {
        id: localeColumn
        anchors.bottom: parent.bottom
        anchors.top: formatText.bottom
        anchors.topMargin: parent.height * 0.025
        anchors.left: formatText.right
        width: parent.width * 0.5
        spacing: parent.height * 0.01
        Row {
            id: regionFormat
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height * 0.1
            TableKey {
                text: qsTr("Region")
            }
            TableValue {
                text: root.currentRegion.nativeCountryName
            }
        }
        Row {
            id: shortDateFormat
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height * 0.1
            TableKey {
                text: qsTr("Short date:")
            }
            TableValue {
                text: root.currentRegion.dateFormat(1)
            }
        }
        Row {
            id: shortTimeFormat
            anchors.left: parent.left

            anchors.right: parent.right
            height: parent.height * 0.1
            TableKey {
                text: qsTr("Short time:")
            }
            TableValue {
                text: root.currentRegion.timeFormat(1)
            }
        }
        Row {
            id: currencyFormat
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height * 0.1
            TableKey {
                text: qsTr("Currency:")
            }
            TableValue {
                text: root.currentRegion.currencySymbol(1)
            }
        }
        Row {
            id: firstDayFormat
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height * 0.1
            TableKey {
                text: qsTr("First day of week:")
            }
            TableValue {
                text: root.currentRegion.firstDayOfWeek
            }
        }
    }
}
