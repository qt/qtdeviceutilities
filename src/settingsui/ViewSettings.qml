/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt for Device Creation.
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

Item {
    id: viewSettings

    property real pageMargin: 0.03 * Math.min(window.width, window.height)
    property real spacing: pageMargin * 0.5
    property string appFont: "TitilliumWeb"
    property string backgroundColor: "#09102b"
    property string borderColor: "#9d9faa"
    property string buttonGreenColor: "#41cd52"
    property string buttonGrayColor: "#9d9faa"
    property string buttonActiveColor: "#216729"
    property string scrollBarColor: "#41cd52"
    property bool usbEthernetSettingVisible: false
}
