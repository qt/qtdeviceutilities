/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
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

FocusScope {
    id: textBase

    property var editor
    property bool previewTextActive: !editor.activeFocus && text.length === 0
    property int fontPixelSize: 32
    property string previewText
    property string enterKeyText
    property bool enterKeyEnabled: true
    property alias mouseParent: mouseArea.parent

    implicitHeight: editor.height + 12

    signal enterKeyClicked

    Keys.onReleased: {
        if (event.key === Qt.Key_Return)
            enterKeyClicked()
    }

    Rectangle {
        // background
        radius: 5.0
        anchors.fill: parent
        color: Qt.rgba(1.0, 1.0, 1.0, 0.2)
        border { width: editor.activeFocus ? 2 : 0; color: "#CCCCCC" }
    }
    Text {
        id: previewText

        y: 8
        color: "#CCCCCC"
        visible: previewTextActive
        text: textBase.previewText
        font.pixelSize: 28
        anchors { left: parent.left; right: parent.right; margins: 12 }

    }
    MouseArea {
        id: mouseArea

        z: 1
        parent: textBase
        anchors.fill: parent
        onClicked: {
            var positionInEditor = mapToItem(editor, mouseX, mouseY)
            var cursorPosition = editor.positionAt(positionInEditor.x, positionInEditor.y)
            editor.cursorPosition = cursorPosition
            editor.forceActiveFocus()
            Qt.inputMethod.show()
        }
    }
}
