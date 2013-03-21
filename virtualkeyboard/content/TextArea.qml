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

TextBase {
    id: textArea

    property alias color: textEdit.color
    property alias text: textEdit.text
    property alias textWidth: textEdit.width
    property alias readOnly: textEdit.readOnly
    property alias inputMethodHints: textEdit.inputMethodHints

    editor: textEdit

    Repeater {
        model: Math.floor((parent.height - 30) / editor.cursorRectangle.height)
        Rectangle {
            x: 8
            y: (index+1)*editor.cursorRectangle.height+6
            height: 1; width: textArea.width-24
            color: Qt.rgba(1.0, 1.0, 1.0, 0.5)
        }
    }
    TextEdit {
        id: textEdit

        property alias enterKeyText: textArea.enterKeyText
        property alias enterKeyEnabled: textArea.enterKeyEnabled

        y: 6
        focus: true
        color: "#EEEEEE"
        wrapMode: TextEdit.Wrap
        cursorVisible: activeFocus
        height: Math.max(implicitHeight, 60)
        font.pixelSize: textArea.fontPixelSize
        selectionColor: Qt.rgba(1.0, 1.0, 1.0, 0.5)
        selectedTextColor: Qt.rgba(0.0, 0.0, 0.0, 0.8)
        anchors { left: parent.left; right: parent.right; margins: 12 }
        cursorDelegate: Rectangle {
            width: 2
            opacity: 0.7
            color: "#EEEEEE"
            visible: textEdit.activeFocus
        }

        onActiveFocusChanged: if (!activeFocus) deselect()
    }
}
