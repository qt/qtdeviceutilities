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
import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles.Flat 1.0 as Flat
import QtQuick.XmlListModel 2.0
import com.theqtcompany.settings.common 1.0

ApplicationWindow {
    id: root
    title: qsTr("Device settings")
    width: 800
    height: 600
    visible: true
    property var service

    SystemPalette { id: systemPalette; colorGroup: SystemPalette.Active }

    toolBar:ToolBar {
        id: titlebar
        property string title: ""
        implicitWidth: parent.width
        implicitHeight: Math.round(40 * Flat.FlatStyle.scaleFactor)
        Item {
            anchors.fill: parent

            ToolButton {
               iconSource: "../icons/Chevron-left_black_1x.png"
               text: ""
               visible: stackView.depth > 1
               height: parent.height
               width: parent.height
               onClicked: stackView.pop();
            }
            TextLabel {
                id: titleText
                font.pixelSize: Math.round(16 * Flat.FlatStyle.scaleFactor)
                anchors.fill: parent
                text: stackView.currentItem.title
            }


            ToolButton {
               iconSource: "../icons/Power_black_1x.png"
               text: ""
               visible: stackView.depth == 1
               height: parent.height
               width: parent.height
               anchors.right: parent.right
               onClicked: {
                   menu.__xOffset = Math.round(-100 * Flat.FlatStyle.scaleFactor);
                   menu.popup();
               }
            }
        }
    }

    Menu {
        id: menu
        title: ""
        enabled: false

        MenuItem {
            text: qsTr("Reboot")
            onTriggered: B2QtDevice.reboot()
        }

        MenuItem {
            text: qsTr("Shutdown")
            onTriggered: B2QtDevice.powerOff()
        }
    }

    StackView {
        id: stackView
        initialItem: mainView
        anchors.top: parent.top
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
                    XmlRole { name: "plugin"; query: "plugin/string()"}
                    XmlRole { name: "view"; query: "view/string()"}
                }

                GridView {
                    id: grid
                    anchors.fill: parent
                    anchors.margins: Math.round(40 * Flat.FlatStyle.scaleFactor)
                    cellHeight: Math.round(133 * Flat.FlatStyle.scaleFactor)  + spacing
                    cellWidth: Math.round(117 * Flat.FlatStyle.scaleFactor) + spacing
                    property int spacing: Math.round(20 * Flat.FlatStyle.scaleFactor)

                    model: xmlModel
                    delegate:
                        MouseArea {
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
                                anchors.topMargin: Math.round(10 * Flat.FlatStyle.scaleFactor)
                                Image {
                                    id: image
                                    source: icon
                                    anchors.horizontalCenter: parent.horizontalCenter
                                }

                                TextLabel {
                                    text: title
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    font.pixelSize: Math.round(16 * Flat.FlatStyle.scaleFactor)
                                }
                            }

                       onClicked: {
                           var url = 'import ' + plugin + '; ' + view + ' {}';
                           stackView.push({item: Qt.createQmlObject(url,stackView)});
                           titlebar.title = title
                       }
                    }
                }
            }
        }
    }
}
