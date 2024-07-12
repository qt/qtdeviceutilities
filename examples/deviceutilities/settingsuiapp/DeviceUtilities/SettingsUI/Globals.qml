// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
pragma Singleton
import QtQuick
import QtDeviceUtilities.NetworkSettings

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
