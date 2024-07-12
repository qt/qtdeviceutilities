// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: root
    spacing: 10
    property alias model: repeater.model
    property bool modified: false
    property alias title: label.text

    Label {
        id: label
        Layout.preferredWidth: content.titleWidth
        horizontalAlignment: Text.AlignRight
        Layout.alignment: Qt.AlignTop
        Layout.topMargin: 10
    }
    ColumnLayout {
        spacing: 10
        Layout.fillWidth: true

        Repeater {
            id: repeater
            visible: count > 0
            RowLayout {
                spacing: 10
                TextField {
                    text: edit
                    Layout.fillWidth: true
                    onEditingFinished: {
                        root.modified = true;
                        edit = text;
                    }
                }
                Button {
                    Layout.preferredWidth: height
                    text: "-"
                    visible: repeater.count > 0
                    onClicked: {
                        root.modified = true;
                        root.model.remove(index);
                    }
                }
                Button {
                    Layout.preferredWidth: height
                    visible: index === repeater.count - 1
                    text: "+"
                    onClicked: root.model.append("")

                    Layout.alignment: Qt.AlignRight
                }
            }
        }
        RowLayout {
            visible: repeater.count === 0
            spacing: 10
            TextField {
                id: nameServerEntryItem
                Layout.fillWidth: true
                text: ""
                onAccepted: root.model.append(text)
            }
            Button {
                Layout.preferredWidth: height
                text: "+"
                onClicked: nameServerEntryItem.accepted()
            }
        }
    }
}
