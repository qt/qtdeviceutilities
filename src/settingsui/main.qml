/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the FOO module of the Qt Toolkit.
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
import Qt.labs.controls 1.0
import Qt.labs.controls.material 1.0
import Qt.labs.controls.universal 1.0
import Qt.labs.settings 1.0
import QtQuick.XmlListModel 2.0
import QtQuick.Enterprise.VirtualKeyboard 2.0
import com.theqtcompany.localdevice 1.0
import "common"

ApplicationWindow {
    id: root
    title: qsTr("Device settings")
    width: 800
    height: 600
    visible: true
    property var service

    header: ToolBar {
        id: titlebar
        property string title: ""

        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                label: Image {
                    source: "../icons/Chevron-left_black_1x.png"
                    anchors.centerIn: parent
                }
                visible: stackView.depth > 1
                onClicked: stackView.pop();
            }
            Label {
                id: titleText
                font.pixelSize: 20
                text: stackView.currentItem.title
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                anchors.centerIn: parent
            }
            ToolButton {
                label: Image {
                    source: "../icons/Power_black_1x.png"
                    anchors.centerIn: parent
                }
                anchors.right: parent.right

                visible: stackView.depth === 1
                onClicked: menu.open();

                Menu {
                    id: menu
                    x: parent.width - width

                    MenuItem {
                        text: qsTr("Reboot")
                        onTriggered: LocalDevice.reboot()
                    }
                    MenuItem {
                        text: qsTr("Shutdown")
                        onTriggered: LocalDevice.powerOff()
                    }
                }
            }
        }
    }
    StackView {
        id: stackView
        initialItem: mainView
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Behavior on anchors.topMargin { NumberAnimation { duration: 250; easing.type: Easing.InOutQuad }}

        Component {
            id: mainView

            Item {
                property string title: qsTr("Device Settings")

                XmlListModel {
                    id: xmlModel
                    source: "settingsview.xml"
                    query: "/xml/settings/item"
                    XmlRole { name: "title"; query: "title/string()"}
                    XmlRole { name: "icon"; query: "icon/string()"}
                    XmlRole { name: "view"; query: "view/string()"}
                    XmlRole { name: "path"; query: "path/string()"}
                }
                GridView {
                    id: grid
                    anchors.fill: parent
                    anchors.margins: 40
                    cellHeight: 133 + spacing
                    cellWidth: 117 + spacing
                    model: xmlModel
                    property int spacing: 20
                    delegate: MouseArea {
                        width: grid.cellWidth - grid.spacing
                        height: grid.cellHeight - grid.spacing

                        Rectangle {
                            anchors.fill: parent
                            color: "#d6d6d6"
                            radius: 5
                            visible: pressed
                        }
                        Column {
                            anchors.fill: parent
                            anchors.topMargin: 10
                            Image {
                                id: image
                                source: icon
                                anchors.horizontalCenter: parent.horizontalCenter
                                width: grid.cellWidth
                                fillMode: Image.PreserveAspectFit
                            }
                            Label {
                                text: title
                                anchors.horizontalCenter: parent.horizontalCenter
                                font.pixelSize: grid.cellHeight * .1
                            }
                        }
                        onClicked: {
                            var component = Qt.createComponent(path + '/' +view+'.qml');
                            stackView.push(component.createObject(stackView));
                            titlebar.title = title
                        }
                    }
                }
            }
        }
    }

    property var inputItem: InputContext.inputItem

    HandwritingInputPanel {

        id: handwritingInputPanel
        anchors.fill: parent
        inputPanel: inputPanel

        Rectangle {
            z: -1
            anchors.fill: parent
            color: "black"
            opacity: 0.10
        }

        onAvailableChanged: {
            if (!available)
                inputPanel.ensureVisible()
        }
    }
    Item {
        visible: handwritingInputPanel.enabled && Qt.inputMethod.visible
        anchors { left: parent.left; top: parent.top; right: parent.right; bottom: inputPanel.top; }

        HandwritingModeButton {
            id: handwritingModeButton
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.margins: 10
            floating: true
            flipable: true
            width: 76
            height: width
            state: handwritingInputPanel.state
            onClicked: handwritingInputPanel.active = !handwritingInputPanel.active
            onDoubleClicked: handwritingInputPanel.available = !handwritingInputPanel.available
        }
    }
    InputPanel {
        id: inputPanel
        y: active ? parent.height - inputPanel.height : parent.height
        anchors.left: parent.left
        anchors.right: parent.right

        Behavior on y { NumberAnimation {duration: 250; easing.type: Easing.InOutQuad}}

        property var inputItem: InputContext.inputItem

        onInputItemChanged: {
            if (handwritingInputPanel.available)
                return;

            if (inputItem) {
                ensureVisible()
            }
        }

        onActiveChanged: {
            if (!active) {
                stackView.anchors.topMargin = 0

                if (!handwritingInputPanel.available && inputItem)
                    inputItem.focus = false
            }
        }

        function ensureVisible() {
            var mapped = inputItem.mapToItem(handwritingInputPanel, 0,0)
            if (mapped.y > (handwritingInputPanel.height-inputPanel.height-40)) {
                stackView.anchors.topMargin = -inputPanel.height
            }
        }
    }
}
