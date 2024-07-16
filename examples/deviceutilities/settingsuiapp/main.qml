// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtQuick.Window
import QtQuick.VirtualKeyboard
import DeviceUtilities
import DeviceUtilities.SettingsUI

Window {
    id: window
    visible: true
    width: ApplicationSettings.width
    height: ApplicationSettings.height

    Item {
        id: root
        width: window.width
        height: window.height

        SettingsUI {
            id: settingsUI
            anchors.fill: parent
            anchors.bottomMargin: parent.height - inputPanel.y
        }

        /*  Handwriting input panel for full screen handwriting input.

        This component is an optional add-on for the InputPanel component, that
        is, its use does not affect the operation of the InputPanel component,
        but it also can not be used as a standalone component.

        The handwriting input panel is positioned to cover the entire area of
        application. The panel itself is transparent, but once it is active the
        user can draw handwriting on it.
        */

        HandwritingInputPanel {
            z: 79
            id: handwritingInputPanel
            anchors.fill: parent
            inputPanel: inputPanel
            Rectangle {
                z: -1
                anchors.fill: parent
                color: "black"
                opacity: 0.10
            }
        }

        /*  Container area for the handwriting mode button.

        Handwriting mode button can be moved freely within the container area.
        In this example, a single click changes the handwriting mode and a
        double-click changes the availability of the full screen handwriting input.
        */
        Item {
            z: 89
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

        /*  Keyboard input panel.
            The keyboard is anchored to the bottom of the application.
        */
        InputPanel {
            id: inputPanel
            z: 99
            y: root.height
            anchors.left: root.left
            anchors.right: root.right

            states: State {
                name: "visible"
                /*  The visibility of the InputPanel can be bound to the Qt.inputMethod.visible property,
                but then the handwriting input panel and the keyboard input panel can be visible
                at the same time. Here the visibility is bound to InputPanel.active property instead,
                which allows the handwriting panel to control the visibility when necessary.
            */
                when: inputPanel.active
                PropertyChanges {
                    target: inputPanel
                    y: root.height - inputPanel.height
                }
            }
            transitions: Transition {
                from: ""
                to: "visible"
                reversible: true
                ParallelAnimation {
                    NumberAnimation {
                        properties: "y"
                        duration: 250
                        easing.type: Easing.InOutQuad
                    }
                }
            }
            AutoScroller {}
        }
    }
}
