/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
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
pragma Singleton
import QtQuick

QtObject {
    id: globals
    readonly property string appFont: "TitilliumWeb"
    readonly property color backgroundColor: "#09102b"
    readonly property color borderColor: "#9d9faa"
    readonly property color buttonGreenColor: "#41cd52"
    readonly property color buttonGrayColor: "#9d9faa"
    readonly property color buttonActiveColor: "#216729"
    readonly property color scrollBarColor: "#41cd52"

    readonly property real spacing: 0.5
    readonly property real titleFontSize: 0.04
    readonly property real subTitleFontSize: 0.035
    readonly property real valueFontSize: 0.025
    readonly property real fieldHeight: 0.07
    readonly property real fieldTextHeight: 0.05
    readonly property real buttonHeight: 0.05

    function margin(width) {
        return (width / 3 * 2) * 0.05;
    }
}
