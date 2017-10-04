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
import "../common"
import QtGraphicalEffects 1.0
import QtDeviceUtilities.QtButtonImageProvider 1.0

Item {
    id: root
    // QTBUG-63598: property var currentRegion: Qt.locale(LocaleManager.locale)
    property var currentRegion: Qt.locale("en_EN")

    Row {
        id: backRow
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width * 0.2
        height: parent.height * 0.04
        Image {
            id: languageBackIcon
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height * 0.8
            width: height
            fillMode: Image.PreserveAspectFit
            source: "../newIcons/back_icon.svg"

            ColorOverlay {
                source: languageBackIcon
                anchors.fill: languageBackIcon
                color: "#41cd52"
                visible: true
            }
            MouseArea {
                anchors.fill: parent
                onClicked: settingsLoader.source = "qrc:/locale/Language.qml"
            }
        }
        Text {
            anchors.top: parent.top
            height: parent.height
            anchors.verticalCenter: parent.verticalCenter
            verticalAlignment: Text.AlignVCenter
            fontSizeMode: Text.Fit
            minimumPixelSize: 1
            font.pixelSize: height
            color: "#41cd52"
            text: "Back"
            font.family: appFont
            MouseArea {
                anchors.fill: parent
                onClicked: settingsLoader.source = "qrc:/locale/Language.qml"
            }
        }
    }

    Text {
        id: changeLanguageText
        anchors.top: backRow.bottom
        anchors.topMargin: parent.height * 0.05
        anchors.left: parent.left
        fontSizeMode: Text.Fit
        minimumPixelSize: 1
        font.pixelSize: parent.height * 0.05
        color: "white"
        text: qsTr("Change Language")
        font.family: appFont
        font.styleName: "Bold"
    }
    Rectangle {
        id: btmLine
        anchors.top: changeLanguageText.bottom
        anchors.topMargin: parent.height * 0.025
        anchors.left: changeLanguageText.left
        width: parent.width * 0.275
        height: parent.height * 0.005
    }

    ColumnLayout {
        id: content
        anchors.top: btmLine.bottom
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.topMargin: parent.height * 0.025
        RowLayout {
            spacing: 10
            Layout.fillHeight: false
            Layout.preferredHeight: root.height * 0.075
            TextField {
                id: country
                Layout.fillHeight: true
                Layout.preferredWidth: root.width * 0.5
                font.pixelSize: parent.height * 0.5
                color: "white"
                text: ""
                placeholderText: "Search..."
                font.family: appFont
                font.styleName: "Light"
                onTextChanged: LocaleFilter.filter = country.text
                Layout.alignment: Qt.AlignVCenter
                background: Rectangle {
                    color: "transparent"
                    border.color: "#9d9faa"
                    border.width: 2
                }
            }
        }

        CustomTableView {
            id: localeTableView
            headerTexts: [qsTr("Language"), qsTr("Country")]
            roleNames: ["language", "country"]
            model: LocaleFilter
            onClicked: {
                var val = model.itemFromRow(index);
                if (val !== "") {
                    LocaleManager.locale = val;
                    settingsLoader.source = "qrc:/locale/Language.qml"
                }
            }
        }
    }
    Component.onCompleted: {
        country.text = LocaleFilter.filter
        var n = LocaleFilter.indexForCountry(root.currentRegion.nativeCountryName)
        localeTableView.localeIndex = n
    }

    Connections {
        target: LocaleFilter.sourceModel
        onReady: {
            var n = LocaleFilter.indexForCountry(root.currentRegion.nativeCountryName)
            localeTableView.localeIndex = n
        }
    }
}
