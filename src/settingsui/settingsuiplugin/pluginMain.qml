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
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.0
import Qt.labs.settings 1.0
import QtQuick.XmlListModel 2.0
import QtQuick.Enterprise.VirtualKeyboard 2.0
import QtDeviceUtilities.LocalDeviceSettings 1.0

Rectangle {
    id: root
    anchors.fill: parent
    color: "white"
    visible: true
    property var service

    ToolBar {
        id: titlebar
        property string title: ""
        width: parent.width
        height: 40
        z: 2

        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                contentItem: Image {
                    fillMode: Image.PreserveAspectFit
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
                contentItem: Image {
                    fillMode: Image.PreserveAspectFit
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
        anchors.top: titlebar.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

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
                            var component = Qt.createComponent(path+'/'+view+'.qml');
                            stackView.push(component.createObject(stackView));
                        }
                    }
                }
            }
        }
    }
}
