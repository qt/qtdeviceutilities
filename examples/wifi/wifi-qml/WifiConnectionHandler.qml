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
import B2Qt.Wifi 1.0

//! [0]
Item {
    WifiConfiguration {
        id: localConfig
        ssid: "network-for-my-device"
        passphrase: "password123"
        protocol: "WPA2"
    }

    Connections {
        target: WifiManager
        onBackendStateChanged: {
            if (WifiManager.backendState === WifiManager.Running)
                WifiManager.connect(localConfig)
        }
        onNetworkStateChanged: {
            if (WifiManager.networkState === WifiManager.Connected)
                print("successfully connected to: " + WifiManager.currentSSID)
        }
    }

    Component.onCompleted: {
        if (WifiManager.backendState === WifiManager.Running) {
            WifiManager.connect(localConfig)
        } else {
            WifiManager.start()
        }
    }
}
//! [0]
