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
import QtDeviceUtilities.LocaleSettings 1.0
import QtQml 2.2

Item {
    id: root
    property string title: qsTr("Language and Region")

    GroupBox {
        id: groupBox
        title: qsTr("Regional Format")
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.margins: 20
        property var currentRegion: Qt.locale(LocaleManager.locale)

        ColumnLayout {
            width: parent.width

            Component {
                id: regionSelect
                RegionSelect {}
            }
            GroupBox {
                width: parent.width
                title: {
                    if (groupBox.currentRegion.name === "C" || groupBox.currentRegion.name === "POSIX") {
                        return qsTr("Default");
                    }
                    else if (groupBox.currentRegion.name !== "") {
                        return qsTr("%L1/%L2").arg(groupBox.currentRegion.nativeLanguageName).arg(groupBox.currentRegion.nativeCountryName)
                    }
                    else {
                        return qsTr("Region not set");
                    }
                }
                ColumnLayout {
                    spacing: 10
                    Layout.fillWidth: true
                    width: parent.width

                    Label {
                        text: Date().toLocaleString(groupBox.currentRegion)
                    }
                    Label {
                        text: Number(2343.34).toLocaleString(groupBox.currentRegion)
                    }
                    Label {
                        text: Number(41334.34).toLocaleCurrencyString(groupBox.currentRegion)
                    }
                }
            }
            Button {
                text: qsTr("Change region")
                onClicked: stackView.push(regionSelect)
            }
        }
    }
}
