/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.0
import "components"
import Boot2Qt.InputContext 1.0
import Qt.labs.folderlistmodel 2.0

Image {
    id: keyboard

    property var activeKey: 0
    property int localeIndex: 1
    property string locale: layoutsModel.count > 0 ? layoutsModel.get(localeIndex, "fileBaseName") : "en_GB"
    property bool active: Qt.inputMethod.visible
    property bool uppercased: uppercaseOnly ? true : (lowercaseOnly ? false : InputContext.shift || InputContext.capsLock)
    property bool uppercaseOnly: InputContext.inputMethodHints & Qt.ImhUppercaseOnly
    property bool lowercaseOnly: InputContext.inputMethodHints & Qt.ImhLowercaseOnly
    property bool numberMode: InputContext.inputMethodHints & (Qt.ImhDialableCharactersOnly | Qt.ImhFormattedNumbersOnly | Qt.ImhDigitsOnly)
    property bool symbolMode

    z: 1
    source: "images/keypad.png"
    fillMode: Image.Stretch
    width: parent.width
    height: keyboardLayout.height+10
    y: Qt.inputMethod.visible ? parent.height - height : parent.height
    visible: Qt.inputMethod.visible || Qt.inputMethod.animating;

    onLocaleChanged: InputContext.setLocale(locale)
    onUppercasedChanged: if (Qt.inputMethod.visible) InputContext.handleUppercasing(uppercased)

    function changeInputLanguage() {
        localeIndex = (localeIndex+1) % layoutsModel.count
    }

    Behavior on y {
        SequentialAnimation {
            ScriptAction {
                script: {
                    if (Qt.inputMethod.visible) {
                        keyboard.symbolMode = false
                        InputContext.handleUppercasing(uppercased)
                    }
                    InputContext.setAnimating(true)
                }
            }
            NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
            ScriptAction { script: InputContext.setAnimating(false) }
        }
    }
    AutoScroller {}
    FolderListModel {
        id: layoutsModel
        folder: "layouts"
        nameFilters: ["*_*.qml"]
    }
    PressedKey {}
    InputEngine { id: inputEngine }
    AlternativeKeys { id: alternativeKeys }
    Timer { id: releaseInaccuracyTimer; interval: 500 }
    CharacterPreviewBubble { enabled: keyboardMouse.pressed && !alternativeKeys.active }
    Binding {
        target: InputContext
        property: "keyboardRectangle"
        value: Qt.rect(keyboard.x, keyboard.y, keyboard.width, keyboard.height)
    }
    Column {
        id: keyboardLayout
        anchors.centerIn: parent
        Loader {
            active: false
            visible: keyboard.symbolMode && !keyboard.numberMode
            source: "layouts/symbols.qml"
            onVisibleChanged:  if (visible) active = true
            Component.onCompleted:  if (visible) active = true
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Loader {
            active: false
            visible: keyboard.numberMode
            source: "layouts/numbers.qml"
            onVisibleChanged:  if (visible) active = true
            Component.onCompleted:  if (visible) active = true
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Loader {
            visible: !keyboard.symbolMode && !keyboard.numberMode
            source: "layouts/" + keyboard.locale +".qml"
        }
    }
    MouseArea {
        id: keyboardMouse

        property var initialKey
        property var previousKey
        property bool dragSymbolMode
        property real pressAndHoldMargin: 6
        property real releaseMargin: 18

        anchors.fill: keyboardLayout

        function press(key) {
            if (key && key.enabled) {
                if (key.key != Qt.Key_unknown || key.text.length > 0)
                    inputEngine.sendKeyPress(key.key, keyboard.uppercased ? key.text.toUpperCase() : key.text, keyboard.uppercased ? Qt.ShiftModifier : 0, key.repeat)
            }
        }
        function release(key) {
            if (key && key.enabled) {
                if (key.key != Qt.Key_unknown || key.text.length > 0)
                    inputEngine.sendKeyRelease(key.key, keyboard.uppercased ? key.text.toUpperCase() : key.text, keyboard.uppercased ? Qt.ShiftModifier : 0)
                key.clicked()
            } else if (initialKey !== undefined) // rollback
                InputContext.preeditText = InputContext.preeditText.substr(0, InputContext.preeditText.length-1)
        }
        function click(key) {
            if (key && key.enabled) {
                if (key.key != Qt.Key_unknown || key.text.length > 0)
                    inputEngine.sendKeyClick(key.key, keyboard.uppercased ? key.text.toUpperCase() : key.text, keyboard.uppercased ? Qt.ShiftModifier : 0, key.repeat)
                key.clicked()
            }
        }
        function setActiveKey(activeKey) {
            if (keyboard.activeKey)
                keyboard.activeKey.active = false
            keyboard.activeKey = activeKey
            if (keyboard.activeKey)
                keyboard.activeKey.active = true
        }
        function keyOnMouse(mx, my) {
            var parentItem = keyboardLayout
            var child = parentItem.childAt(mx, my)
            while (child != null) {
                var position = parentItem.mapToItem(child, mx, my)
                mx = position.x; my = position.y
                parentItem = child
                child = parentItem.childAt(mx, my)
                if (child && child.key != undefined)
                    return child
            }
            return undefined
        }
        function hitInitialKey(mouseX, mouseY, margin) {
            if (!initialKey)
                return false
            var position = initialKey.mapFromItem(keyboardMouse, mouseX, mouseY)
            return (position.x > -margin
                    && position.y > -margin
                    && position.x < initialKey.width + margin
                    && position.y < initialKey.height + margin)
        }
        function reset() {
            alternativeKeys.close()
            previousKey = keyboard.activeKey
            setActiveKey(0)
            if (dragSymbolMode)
                keyboard.symbolMode = false
            dragSymbolMode = false
        }

        onPressed: {
            releaseInaccuracyTimer.start()
            initialKey = keyOnMouse(mouseX, mouseY)
            setActiveKey(initialKey ? initialKey : 0)
            press(initialKey)
        }
        onPositionChanged: {
            if (alternativeKeys.active) {
                alternativeKeys.move(mouseX)
            } else {
                var key = keyOnMouse(mouseX, mouseY)
                if (key !== keyboard.activeKey) {
                    setActiveKey(key ? key : 0)
                }
            }
        }
        onPressAndHold: {
            var key = hitInitialKey(mouseX, mouseY, pressAndHoldMargin) ? initialKey : keyboard.activeKey
            if (key != undefined && key  == initialKey) {
                alternativeKeys.open(key)
                if (!alternativeKeys.active && key.key == Qt.Key_Context1) {
                    dragSymbolMode = true
                    keyboard.symbolMode = true
                }
            }
        }
        onClicked: {}
        onReleased: {
            var key = keyOnMouse(mouseX, mouseY)
            if (alternativeKeys.active) {
                alternativeKeys.move(mouseX)
                alternativeKeys.clicked()
            } else if (dragSymbolMode) {
                var key = keyOnMouse(mouseX, mouseY)
                release(initialKey)
                click(key)
            } else {
                release(releaseInaccuracyTimer.running && hitInitialKey(mouseX, mouseY, releaseMargin) ? initialKey : keyOnMouse(mouseX, mouseY))
            }
            reset()
        }
        onCanceled: reset()
    }
}
