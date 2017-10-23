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
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

ComboBox {
    id: comboBox
    property int itemsVisible: 5
    indicator: Image {
        id: indicatorImage
        height: comboBox.height * 0.4
        anchors.right: comboBox.right
        anchors.rightMargin: pluginMain.margin
        anchors.verticalCenter: comboBox.verticalCenter
        fillMode: Image.PreserveAspectFit
        rotation: -90
        source: "../newIcons/back_icon.svg"
    }
    ColorOverlay {
        id: indicatorOverlay
        source: indicatorImage
        anchors.fill: indicatorImage
        color: viewSettings.buttonGreenColor
        visible: false
        rotation: -90
    }
    contentItem: Text {
        anchors.left: comboBox.left
        anchors.leftMargin: pluginMain.margin
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: pluginMain.valueFontSize
        font.family: appFont
        font.styleName: "SemiBold"
        color: "white"
        elide: Text.ElideRight
        text: comboBox.displayText
    }
    background: Rectangle {
        color: "transparent"
        border.color: viewSettings.borderColor
        border.width: comboBox.visualFocus ? 2 : 1
    }
    popup: Popup {
        y: comboBox.height -1
        width: comboBox.width
        height: comboBox.height * comboBox.itemsVisible
        padding: 1

        contentItem: ListView {
            id: listView
            clip: true
            model: comboBox.popup.visible ? comboBox.delegateModel : null
            currentIndex: comboBox.currentIndex
            onCurrentIndexChanged: positionViewAtIndex(listView.currentIndex, ListView.beginning)
        }
        onOpened: {
            listView.currentIndex = comboBox.currentIndex
            indicatorImage.visible = false
            indicatorOverlay.visible = true
        }
        onClosed: {
            indicatorImage.visible = true
            indicatorOverlay.visible = false
        }
        background: Rectangle {
            border.color: viewSettings.borderColor
            color: viewSettings.backgroundColor
            opacity: 0.95
        }
    }
}
