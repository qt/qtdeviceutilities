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
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles.Flat 1.0 as Flat
import QtQml.Models 2.1

Item {
    id: control
    property alias popupParent: popupView.parent
    property bool popupVisible: popupView.visible
    property int selectedIndex: -1
    property alias model: comboboxModel.model
    property string textRole: "text"
    property bool modified: false
    property alias popupVisibility: popupView.visible
    property alias textValue: label.text
    property int popupHeight: comboboxModel.items.count * height
    property bool enabled: true
    property bool editable: true
    property Component delegate

    implicitWidth: Math.round(100 * Flat.FlatStyle.scaleFactor)
    implicitHeight: Math.round(26 * Flat.FlatStyle.scaleFactor)

    Component.onCompleted: {
        //Find root window for parent
        var p = control.parent
        while (p.parent) {
            p = p.parent;
        }
        popupParent = p;
    }

    function hide() {
        popupView.visible = false
        button.checked = false
    }

    function setSelectIndexToVal(value, role) {
        if (model) {
            for (var i=0, len=comboboxModel.items.count;i<len;i++) {
                if (comboboxModel.items.get(i).model[role] === value) {
                    selectedIndex = i;
                    return;
                }
            }
        }
        selectedIndex = -1;
    }

    Button {
         id: button
         checkable: true
         enabled: true
         anchors.fill: parent
         checked: popupView.visible
         onClicked: popupView.visible = !popupView.visible

         TextLabel {
            id: label
            anchors.fill: parent
            anchors.margins: 10 * Flat.FlatStyle.scaleFactor
            color: button.hovered || button.checked ? Flat.FlatStyle.selectedTextColor:Flat.FlatStyle.styleColor
            horizontalAlignment: Text.AlignLeft
            text: selectedIndex>=0 ? comboboxModel.items.count>0 ? comboboxModel.items.get(selectedIndex).model[textRole]:"":""
         }

         Flat.LeftArrowIcon {
             anchors.right: parent.right
             anchors.verticalCenter: parent.verticalCenter
             anchors.rightMargin: Math.round(10 * Flat.FlatStyle.scaleFactor)
             width: Math.round(14 * Flat.FlatStyle.scaleFactor)
             height: Math.round(14 * Flat.FlatStyle.scaleFactor)
             rotation: popupVisibility ? 90 : -90
             color: button.hovered || button.checked ? Flat.FlatStyle.selectedTextColor:Flat.FlatStyle.styleColor
             opacity: control.enabled || control.editable ? 1.0 : 0.2
         }
    }

    Item {
        id: popupView
        y: 0
        x: 0
        height: control.popupHeight
        width: control.width
        visible: false
        onVisibleChanged: {
            if (visible) {
                var pos = control.mapToItem(popupView.parent, 0, 0)
                x = pos.x
                y = pos.y + button.height

                if (y + control.popupHeight > popupParent.height) {
                    y = popupParent.height - control.popupHeight;
                }
            }
        }

        Rectangle {
            anchors.fill: parent
            border.color: Flat.FlatStyle.darkFrameColor
            border.width: 1
            color: "white"

            Column {
                ExclusiveGroup {
                    id: exgroup
                }

                DelegateModel {
                    id: comboboxModel
                    delegate: comboboxDelete
                }

                Repeater {
                    id: comboboxView
                    model: comboboxModel
                }
            }
        }
    }

    Component {
        id: comboboxDelete
        Item {
            id: comboboxViewDelegate
            property bool checked
            width: control.width
            height: control.height

            Loader {
                id: loader
                sourceComponent: delegate
                anchors.fill: parent
                property var modelData: model
                onLoaded : exgroup.bindCheckable(item)
            }

            Connections {
                target: loader.item
                onClicked: {
                    popupView.visible = false;
                    button.checked = false
                    selectedIndex = index
                }
            }
        }
    }
}
