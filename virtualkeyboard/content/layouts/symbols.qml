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
import "../components"

Column {
    property bool secondPage
    onVisibleChanged: if (!visible) secondPage = false

    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        Key {
            key: secondPage ? Qt.Key_unknown : Qt.Key_1
            text: secondPage ? ":)" : "1"
            textPixelSize: secondPage ? 20 : 24
        }
        Key {
            key: secondPage ? Qt.Key_unknown : Qt.Key_2
            text: secondPage ? ":D" : "2"
            textPixelSize: secondPage ? 20 : 24
        }
        Key {
            key: secondPage ? Qt.Key_Dollar : Qt.Key_3
            text: secondPage ? ":(" : "3"
            textPixelSize: secondPage ? 20 : 24
        }
        Key {
            key: secondPage ? Qt.Key_yen : Qt.Key_4
            text: secondPage ? "<3": "4"
            textPixelSize: secondPage ? 18 : 24
        }
        Key {
            key:  Qt.Key_5
            text: secondPage ? "½" : "5"
        }
        Key {
            key: secondPage ?  Qt.Key_Percent : Qt.Key_6
            text: secondPage ? "%": "6"
        }
        Key {
            key: secondPage ? Qt.Key_Less : Qt.Key_7
            text: secondPage ? "<": "7"
        }
        Key {
            key: secondPage ? Qt.Key_Greater : Qt.Key_8
            text: secondPage ? ">" : "8"
        }
        Key {
            key: secondPage ? Qt.Key_BracketLeft : Qt.Key_9
            text: secondPage ? "[" : "9"
        }
        Key {
            key: secondPage ? Qt.Key_BracketRight : Qt.Key_0
            text: secondPage ? "]" : "0"
        }
    }
    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        Key {
            key: secondPage ? Qt.Key_AsciiTilde : Qt.Key_At
            text: secondPage ? "~" : "@"
        }
        Key {
            key: secondPage ? Qt.Key_Agrave : Qt.Key_Asterisk
            text: secondPage ? "`" : "*"
        }
        Key {
            key: secondPage ? Qt.Key_AsciiCircum : Qt.Key_NumberSign
            text: secondPage ? "^" : "#"
        }
        Key {
            key: secondPage ? Qt.Key_Bar : Qt.Key_Plus
            text: secondPage ? "|" : "+"
        }
        Key {
            key: secondPage ? Qt.Key_Underscore : Qt.Key_Minus
            text: secondPage ? "_" : "-"
        }
        Key {
            key: secondPage ? Qt.Key_section : Qt.Key_Equal
            text: secondPage ? "§" : "="
        }
        Key {
            key: secondPage ? Qt.Key_BraceLeft : Qt.Key_ParenLeft
            text: secondPage ? "{" : "("
        }
        Key {
            key: secondPage ? Qt.Key_BraceRight : Qt.Key_ParenRight
            text: secondPage ? "}" : ")"
        }
        Key {
            key: secondPage ? Qt.Key_questiondown : Qt.Key_Question
            text: secondPage ? "¿" : "?"
        }
        Key {
            key: secondPage ? Qt.Key_exclamdown : Qt.Key_Apostrophe
            text: secondPage ? "¡" : "!"
        }
    }
    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        Key {
            width: 47
            displayText: secondPage ? " 2/2" : " 1/2"
            textPixelSize: 20
            functionKey: true
            onClicked: secondPage = !secondPage
        }
        Key {
            key: secondPage ? Qt.Key_guillemotleft : Qt.Key_Period
            text: secondPage ? '«' : "."
        }
        Key {
            key: secondPage ? Qt.Key_guillemotright : Qt.Key_Comma
            text: secondPage ? '»' : ","
        }
        Key {
            key: secondPage ?  Qt.Key_QuoteDbl : Qt.Key_Slash
            text: secondPage ? '"' : "/"
        }
        Key {
            key: secondPage ? Qt.Key_unknown : Qt.Key_Backslash
            text: secondPage ? '“' : "\\"
        }
        Key {
            key: secondPage ? Qt.Key_unknown : Qt.Key_Exclam
            text: secondPage ? '”' : "'"
        }
        Key {
            key: secondPage ? Qt.Key_unknown : Qt.Key_Colon
            text: secondPage ? '„' : ":"
        }
        Key {
            key: secondPage ? Qt.Key_Ampersand : Qt.Key_Semicolon
            text: secondPage ? "&" : ";"
        }
        BackspaceKey {

        }
    }
    Row {
        anchors.horizontalCenter: parent.horizontalCenter
        SymbolModeKey { displayText: "ABC" }
        SpaceKey {}
        HideKeyboardKey {}
        EnterKey {}
    }
}
