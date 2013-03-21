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
import Boot2Qt.InputContext 1.0

Item {
    property var innerFlickable
    property var outerFlickable
    property var inputItem: Qt.inputMethod.inputItem

    onInputItemChanged: {
        if (inputItem) {
            var parent_ = inputItem.parent
            while (parent_) {
                if (parent_.maximumFlickVelocity) {
                    if (innerFlickable) {
                        outerFlickable = parent_
                        break
                    } else {
                        innerFlickable = parent_
                    }
                }
                parent_ = parent_.parent
            }
            delayedLoading.triggered()
        }
    }

    function ensureVisible(flickable) {
        if (Qt.inputMethod.visible && inputItem && flickable && flickable.visible && flickable.interactive) {

            var verticallyFlickable = (flickable.flickableDirection == Flickable.HorizontalAndVerticalFlick || flickable.flickableDirection == Flickable.VerticalFlick
                                       || (flickable.flickableDirection == Flickable.AutoFlickDirection && flickable.contentHeight > flickable.height))
            var horizontallyFlickable = (flickable.flickableDirection == Flickable.HorizontalAndVerticalFlick || flickable.flickableDirection == Flickable.HorizontalFlick
                                         || (flickable.flickableDirection == Flickable.AutoFlickDirection && flickable.contentWidth > flickable.width))

            if (!verticallyFlickable && !horizontallyFlickable)
                return

            var cursorRectangle = flickable.contentItem.mapFromItem(inputItem, inputItem.cursorRectangle.x, inputItem.cursorRectangle.y)

            var oldContentY = flickable.contentY
            if (verticallyFlickable) {
                var scrollMarginVertical = (flickable && flickable.scrollMarginVertical) ? flickable.scrollMarginVertical : 10
                if (flickable.contentY >= cursorRectangle.y  - scrollMarginVertical)
                    flickable.contentY = Math.max(0, cursorRectangle.y  - scrollMarginVertical)
                else if (flickable.contentY + flickable.height <= cursorRectangle.y  + inputItem.cursorRectangle.height + scrollMarginVertical)
                    flickable.contentY = Math.min(flickable.contentHeight - flickable.height, cursorRectangle.y + inputItem.cursorRectangle.height - flickable.height + scrollMarginVertical)
            }
            if (horizontallyFlickable) {
                var scrollMarginHorizontal = (flickable && flickable.scrollMarginHorizontal) ? flickable.scrollMarginHorizontal : 10
                if (flickable.contentX >= cursorRectangle.x - scrollMarginHorizontal)
                    flickable.contentX = Math.max(0, cursorRectangle.x - scrollMarginHorizontal)
                else if (flickable.contentX + flickable.width <= cursorRectangle.x + inputItem.cursorRectangle.width + scrollMarginHorizontal)
                    flickable.contentX = Math.min(flickable.contentWidth - flickable.width, cursorRectangle.x + inputItem.cursorRectangle.width - flickable.width + scrollMarginHorizontal)
            }
        }
    }
    Timer {
        id: delayedLoading
        interval: 10
        onTriggered: {
            ensureVisible(innerFlickable)
            ensureVisible(outerFlickable)
        }
    }
    Connections {
        ignoreUnknownSignals: true
        target: Qt.inputMethod.inputItem && Qt.inputMethod.animating ? Qt.inputMethod : null
        onKeyboardRectangleChanged: delayedLoading.triggered()
    }
    Connections {
        ignoreUnknownSignals: true
        target: Qt.inputMethod.inputItem && Qt.inputMethod.inputItem.activeFocus ? Qt.inputMethod.inputItem : null
        onCursorRectangleChanged: delayedLoading.triggered()
    }
}
