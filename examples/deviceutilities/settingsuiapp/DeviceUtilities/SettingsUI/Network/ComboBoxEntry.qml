// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: root
    spacing: 10
    width: parent.width

    property alias title: label.text
    property alias currentIndex: cb.currentIndex
    property alias delegate: cb.delegate
    property alias textRole: cb.textRole
    property alias model: cb.model
    property alias count: cb.count
    property int titleWidth: -1
    signal activated(int index)
    signal highlighted(int index)

    Label {
        id: label
        Layout.preferredWidth: root.titleWidth
        horizontalAlignment: Text.AlignRight
        Layout.alignment: Qt.AlignVCenter
    }
    ComboBox {
        id: cb
        textRole: "text"
        Layout.fillWidth: true
        onActivated: root.activated(currentIndex)
        onHighlightedIndexChanged: root.highlighted(currentIndex)
    }
}
