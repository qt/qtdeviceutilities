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
#ifndef QWIFIMANAGER_P_H
#define QWIFIMANAGER_P_H

#include "qwifimanager.h"
#include "qwifidevice.h"

#include <QtCore/QString>

QT_BEGIN_NAMESPACE

class QWifiEvent;

class QWifiManagerPrivate
{
    Q_DECLARE_PUBLIC(QWifiManager)
public:
    QWifiManagerPrivate(QWifiManager *manager);
    virtual ~QWifiManagerPrivate();

    // methods
    void setCurrentSSID(const QString &ssid);
    void handleConnected();
    void handleDisconneced();
    void handleAuthenticating(QWifiEvent *event);

    void updateNetworkState(QWifiManager::NetworkState networkState);
    void updateBackendState(QWifiManager::BackendState backendState);
    void updateWifiState();

    QString call(const QString &command);
    bool checkedCall(const QString &command);
    void updateLastError(const QString &error);

    // member variables
    QWifiManager *const q_ptr;
    QWifiController *m_wifiController;
    QWifiNetworkListModel *m_networkListModel;

    int m_scanTimer;
    bool m_scanning;
    QByteArray m_interface;
    QWifiManager::BackendState m_backendState;
    QWifiManager::NetworkState m_networkState;
    QString m_currentSSID;
    QString m_lastError;
};

QT_END_NAMESPACE

#endif // QWIFIMANAGER_P_H
