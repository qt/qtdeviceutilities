/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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
import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    width: settingsList.width
    height: settingsList.height * 0.08

    property bool isSelected: settingsList.currentIndex == index

    Image {
        id: img
        sourceSize.width: parent.width
        sourceSize.height: parent.height
        source: icon
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: parent.width * 0.075
        height: Math.min(parent.width * 0.5, parent.height * 0.6)
        width: height
        fillMode: Image.PreserveAspectFit
        visible: parent.isSelected ? false : true
    }
    ColorOverlay {
        source: img
        anchors.fill: img
        visible: parent.isSelected ? true : false
        color: viewSettings.buttonGreenColor
    }

    Text {
        text: title
        height: parent.height * 0.75
        anchors.right: parent.right
        anchors.left: img.right
        anchors.leftMargin: parent.width * 0.025
        anchors.verticalCenter: parent.verticalCenter
        fontSizeMode: Text.Fit
        minimumPixelSize: 1
        font.pixelSize: Math.min(parent.width * 0.25, parent.height)
        color: parent.isSelected ? viewSettings.buttonGreenColor : "white"
        wrapMode: Text.WordWrap
        verticalAlignment: Text.AlignVCenter
        font.family: appFont
        font.styleName: parent.isSelected ? "Bold" : "Regular"
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            settingsList.currentIndex = index
            settingsLoader.source = path + '/' + view + '.qml'
            titleItem.title = title
        }
    }

    Component.onCompleted: pluginMain.init()
}
