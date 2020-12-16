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
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Qt.labs.settings
import QtDeviceUtilities.NetworkSettings

Rectangle {
    id: pluginMain
    anchors.fill: parent
    visible: true
    color: "#09102b"
    opacity: 0.97

    property var service
    property int margin: (pluginMain.width / 3 * 2) * 0.05
    property int spacing: margin * 0.5
    property int titleFontSize: height * 0.04
    property int subTitleFontSize: height * 0.035
    property int valueFontSize: height * 0.025
    property int fieldHeight: height * 0.07
    property int fieldTextHeight: height * 0.05
    property int buttonHeight: height * 0.05

    signal closed()

    Component.onCompleted: {
        settingsList.currentIndex = 0
        settingsLoader.source = settingsListModel.get(0).path + '/' +
                                settingsListModel.get(0).view + '.qml'
        titleItem.title = settingsListModel.get(0).title
    }

    SettingsHeader {
        id: settingsHeader
        onClicked: pluginMain.closed()
    }

    ListModel {
        id: settingsListModel

        ListElement {
            title: "Network"
            view: "NetworkSettings"
            path: "network"
            icon: "newIcons/network_icon.svg"
        }
    }

    ListView {
        id: settingsList
        objectName: "settingsList"
        anchors.top: settingsHeader.bottom
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width / 3.33
        model: settingsListModel

        delegate: SettingsDelegate {}
    }

    SettingsTitleItem {
        id: titleItem
        anchors.left: settingsList.right
        anchors.right: parent.right
        anchors.top: settingsHeader.bottom
        anchors.leftMargin: pluginMain.margin * 2
    }

    Loader {
        id: settingsLoader
        anchors.top: titleItem.bottom
        anchors.left: settingsList.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.leftMargin: pluginMain.margin * 2
        anchors.rightMargin: pluginMain.margin * 2
    }
}
