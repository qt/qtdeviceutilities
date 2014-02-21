/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://qt.digia.com/
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://qt.digia.com/
**
****************************************************************************/
import QtQuick 2.0
import QtAndroidOmx 1.0

Rectangle {
    id: root
    color: "white"

    Column {
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter

        OmxItem {
            id: omx

            width: root.width * 0.5
            height: width / aspect

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 40
            source: "NativeMedia.ts"

            property real aspect: sourceWidth / (sourceHeight > 0 ? sourceHeight : 1)

            MouseArea {
                anchors.fill: parent
                onClicked: parent.paused = !parent.paused
            }
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Video size: " + omx.sourceWidth + "x" + omx.sourceHeight
        }
    }
}
