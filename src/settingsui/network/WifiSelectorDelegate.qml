/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
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
import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtDeviceUtilities.NetworkSettings 1.0

ItemDelegate {
    id: root
    autoExclusive: true
    property bool connect: connected
    contentItem: Item {
        width: root.width

        Label {
            id: text
            leftPadding: root.spacing
            anchors.left: parent.left
            anchors.top:parent.top
            anchors.right: signalMonitor.left
            anchors.bottom:parent.bottom
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            text: modelData["name"]
        }
        WifiSignalMonitor {
            id: signalMonitor
            anchors.right: parent.right
            height: parent.height
            width: height
            signalStrength: modelData.wirelessConfig["signalStrength"]
            connected: connected
        }
    }
}
