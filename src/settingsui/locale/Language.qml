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
import QtQuick 2.6
import QtQuick.Layouts 1.3
import Qt.labs.controls 1.0
import Qt.labs.controls.material 1.0
import Qt.labs.controls.universal 1.0
import com.theqtcompany.settings.locale 1.0
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
