/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://www.qt.io
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://www.qt.io
**
****************************************************************************/
import QtQuick 2.3
import QtQuick.Window 2.2

Window {
    visible: true
    width: Screen.width
    height: Screen.height
    color: "#D9D9D9"

    WifiScanner {}

    // disable the above line before enabling WifiConnectionHandler
    // WifiConnectionHandler {}
}
