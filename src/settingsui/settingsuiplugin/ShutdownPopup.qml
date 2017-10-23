/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.0

Rectangle {
    id: shutdownPopup
    width: parent.width
    height: parent.height
    color: viewSettings.backgroundColor
    opacity: 0.9

    signal accepted()
    property bool shutdown: false

    function open(sd)
    {
        shutdown = sd
        if (sd) {
            shutDownConfirmText.text = qsTr("Shut down the system?")
            shutDownConfirm.text = qsTr("SHUT DOWN")
        }
        else {
            shutDownConfirmText.text = qsTr("Reboot the system?")
            shutDownConfirm.text = qsTr("REBOOT")
        }

        visible = true;
    }

    function close()
    {
        visible = false;
    }

    Rectangle {
        id: frame
        color: viewSettings.backgroundColor
        border.color: viewSettings.borderColor
        border.width: 3
        anchors.centerIn: parent
        width: parent.width * 0.35
        height: parent.height * 0.4

        Column {
            anchors.centerIn: parent
            spacing: viewSettings.pageMargin

            Text {
                id: shutDownConfirmText
                width: frame.width * 0.75
                height: frame.height * 0.25
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                fontSizeMode: Text.Fit
                minimumPixelSize: 1
                font.pixelSize: frame.width * 0.3
                color: "white"
                font.family: viewSettings.appFont
                font.styleName: "SemiBold"
                text: ""
            }
            QtButton {
                id: shutDownConfirm
                height: frame.height * 0.15
                width: frame.width * 0.45
                anchors.horizontalCenter: parent.horizontalCenter
                text: ""
                onClicked: shutdownPopup.accepted()
            }
            QtButton {
                height: frame.height * 0.15
                width: frame.width * 0.45
                anchors.horizontalCenter: parent.horizontalCenter
                borderColor: "transparent"
                fillColor: viewSettings.buttonGrayColor
                text: qsTr("CANCEL")
                onClicked: shutdownPopup.close()
            }
        }
    }
}
