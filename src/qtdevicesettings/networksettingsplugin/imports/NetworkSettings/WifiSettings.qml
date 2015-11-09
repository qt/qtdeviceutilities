/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles.Flat 1.0 as Flat
import com.theqtcompany.settings.common 1.0
import com.theqtcompany.settings.network 1.0
import com.theqtcompany.settings.wifi 1.0

Item {
    id: root
    anchors.fill: parent
    anchors.margins: Math.round(20 * Flat.FlatStyle.scaleFactor)
    GroupBox {
        id: content
        title: qsTr("Wireless Settings")
        anchors.fill: parent
        Layout.fillWidth: true
        flat: true
        width: parent.width

        Row {
            id: enableSwitch
            width: parent.width
            spacing: Math.round(10 * Flat.FlatStyle.scaleFactor)
            TextLabel {
                width:  root.width*0.382
                horizontalAlignment: Text.AlignRight
                text: NetworkSettingsManager.wifiPowered ? qsTr("Disable Wifi") : qsTr("Enable Wifi")
            }
            Switch {
                checked: NetworkSettingsManager.wifiPowered
                onCheckedChanged: NetworkSettingsManager.wifiPowered = checked
            }
        }

        Item {
            id: wifiManagerContainer
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: enableSwitch.bottom
            anchors.topMargin: Math.round(10 * Flat.FlatStyle.scaleFactor)
            anchors.bottom: parent.bottom
            Loader {
                id: wifiManagerLoader
                asynchronous: true
                sourceComponent: NetworkSettingsManager.wifiPowered ? wifiManager : undefined
            }
        }
    }

    Component {
        id: wifiManager
        WifiManagerView {
            id: wifiSettings
            width: root.width
            visible: true
        }
    }
}
