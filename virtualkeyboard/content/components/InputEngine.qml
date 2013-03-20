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
import Example.PlatformInputContext 1.0

Item {
    property int lastKey
    property string sentenceEndingCharacters: ".;!?"

    Timer {
        // key repeater
        repeat: true
        interval: 600
        running: keyboard.activeKey !== 0 && keyboard.activeKey.repeat

        onRunningChanged: if (!running) interval = 600
        onTriggered: {
            interval = 150
            keyboardMouse.click(keyboard.activeKey)
        }
    }
    Timer {
        id: multiTapTimer

        property int index

        interval: 1200
        onTriggered: {
            index = 0
            InputContext.commit(InputContext.preeditText)
        }
    }
    Item {
        id: shiftHandling

        property bool autoCapitalization: !(InputContext.inputMethodHints & (Qt.ImhNoAutoUppercase | Qt.ImhUppercaseOnly
                                                                         | Qt.ImhLowercaseOnly   | Qt.ImhEmailCharactersOnly
                                                                         | Qt.ImhUrlCharactersOnly | Qt.ImhDialableCharactersOnly
                                                                         | Qt.ImhFormattedNumbersOnly | Qt.ImhDigitsOnly))
        function reset() {
            if (Qt.inputMethod.inputItem) {
                var preferUpperCase = InputContext.inputMethodHints & Qt.ImhPreferUppercase
                InputContext.shift = preferUpperCase
                InputContext.capsLock = preferUpperCase
            }
        }
        function autoCapitalize() {
            if (!Qt.inputMethod.inputItem || !autoCapitalization || InputContext.capsLock)
                return
            var preferLowerCase = InputContext.inputMethodHints & Qt.ImhPreferLowercase
            if (InputContext.cursorPosition == 0)
                InputContext.shift = !preferLowerCase
            else {
                var text = (InputContext.surroundingText + InputContext.preeditText).substr(0, InputContext.cursorPosition).trim()
                if (text.length == 0)
                    InputContext.shift = !preferLowerCase
                else if (text.length > 1 && sentenceEndingCharacters.indexOf(text.charAt(text.length-1)) >= 0)
                    InputContext.shift = !preferLowerCase
                else
                    InputContext.shift = false
            }
        }
        Timer {
            id: delayedLoading
            interval: 10
            onTriggered: {
                shiftHandling.reset()
                shiftHandling.autoCapitalize()
            }
        }
        Connections {
            target: InputContext
            onInputMethodHintsChanged: delayedLoading.restart()
            onFocusEditorChanged: {
                keyboard.symbolMode = false
                delayedLoading.restart()
            }
            onPreeditTextChanged: {
                if (shiftHandling.autoCapitalization && !InputContext.capsLock)
                    InputContext.shift = false
            }
            onCursorPositionChanged: shiftHandling.autoCapitalize()
        }
    }
    function sendKeyClick(key, text, modifiers) {
        sendKeyPress(key, text, modifiers)
        sendKeyRelease(key, text, modifiers)
    }
    function sendKeyPress(key, text, modifiers) {
        lastKey = key
    }
    function sendKeyRelease(key, text, modifiers) {
        var accept = false
        switch (key) {
        case Qt.Key_Enter:
        case Qt.Key_Return:
            break
        case Qt.Key_Space:
            InputContext.commit(" ")
            var text = InputContext.surroundingText.trim()
            if (sentenceEndingCharacters.indexOf(text.charAt(text.length-1)) >= 0)
                keyboard.symbolMode = false
            accept = true
            break
        case Qt.Key_Tab:
        case Qt.Key_Backspace:
            if (InputContext.cursorPosition > 0) {
                if (InputContext.selectedText.length == 0)
                    InputContext.commit("", -1, 1)
                else
                    InputContext.commit("")
            }
            break
        default:
            if (key === Qt.Key_Asterisk && InputContext.inputMethodHints & Qt.ImhDialableCharactersOnly) {
                if (key !=  lastKey) {
                    multiTapTimer.index = 0
                    InputContext.commit(InputContext.preeditText)
                    multiTapTimer.running = false
                }
                multiTapTimer.index = multiTapTimer.running ? (multiTapTimer.index+1) % text.length : 0
                var character = text.charAt(multiTapTimer.index)
                if (multiTapTimer.running)
                    InputContext.preeditText = InputContext.preeditText.substr(0, InputContext.preeditText.length - 1) + character
                else
                    InputContext.preeditText += character
                multiTapTimer.restart()
            } else if (multiTapTimer.running) {
                InputContext.commit(InputContext.preeditText)
                multiTapTimer.stop()
            }
            if (!multiTapTimer.running)
                InputContext.commit(text)
            accept = true
            break
        }
        if (!accept) {
            InputContext.sendKeyClick(key, text, modifiers)
        }
    }
}
