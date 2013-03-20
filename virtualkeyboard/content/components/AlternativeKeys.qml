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

Rectangle {
    property bool active: activeKey != null
    property string keys
    property int primaryKeyCode
    property var activeKey: null
    property real keyWidth: 36
    property real keyMargin: 5

    z: 1
    radius: 4
    height: 60
    parent: keyboard
    visible: active
    width: keyRow.width

    signal clicked

    onClicked: {
        if (active)
            inputEngine.sendKeyRelease(primaryKeyCode,
                                       keyboard.uppercased ? activeKey.text.toUpperCase() : activeKey.text,
                                       keyboard.uppercased ? Qt.ShiftModifier : 0)
    }

    function open(key) {
        keys = key.alternativeKeys
        primaryKeyCode = key.key
        if (keys.length > 0) {
            var position = keyboard.mapFromItem(key, 0, -23)
            var primaryKeyIndex = 0
            while (primaryKeyIndex < keys.length) {
                if (keys.charAt(primaryKeyIndex) == key.text) {
                    break
                }
                primaryKeyIndex++
            }
            if (primaryKeyIndex == keys.length) {
                console.log("AlternativeKeys: active key \"" + key.text + "\" not found in alternativeKeys \"" + keys + ".\"")
                primaryKeyIndex = 0
            }
            x = Math.min(Math.max(keyMargin, position.x-primaryKeyIndex*keyWidth+(key.width-keyWidth)/2-1), keyboard.width-width-keyMargin)
            y = Math.min(position.y-height+keyMargin, keyboard.height-height-keyMargin)
            activeKey = keyRow.children[primaryKeyIndex]
            arrow.x = mapFromItem(key, key.width/2, 0).x - arrow.width/2
        }
    }
    function close() {
        activeKey = null
    }
    function move(mouseX) {
        var oldKey = activeKey
        activeKey = keyRow.children[Math.max(0, Math.min(keys.length-1, Math.floor((mouseX-keyRow.x-x)/keyWidth)))]
    }

    Image {
        id: arrow
        source: "../images/arrow.png"
        anchors.top: parent.bottom
    }
    Row {
        id: keyRow
        anchors.centerIn: parent
        Repeater {
            model: keys.length
            Text {
                id: key
                color: activeKey === key ? "#f04b67" : "black"
                width: keyWidth
                height: keyWidth + 5
                text: keys.charAt(index)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font {
                    pixelSize: 40
                    capitalization: keyboard.uppercased ? Font.AllUppercase : Font.MixedCase
                }
            }
        }
    }
}
