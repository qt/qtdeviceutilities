/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://qt.digia.com/
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://qt.digia.com/
**
****************************************************************************/
#ifndef QWIFIMANAGER_H
#define QWIFIMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtNetwork/QLocalSocket>

#include <cutils/properties.h>

#include "qwifinetworklistmodel.h"

class QWifiManagerEventThread;

class QWifiManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(NetworkState)
    Q_PROPERTY(NetworkState networkState READ networkState NOTIFY networkStateChanged)
    Q_PROPERTY(bool backendReady READ isbackendReady NOTIFY backendReadyChanged)
    Q_PROPERTY(bool scanning READ scanning WRITE setScanning NOTIFY scanningChanged)
    Q_PROPERTY(QString connectedSSID READ connectedSSID NOTIFY connectedSSIDChanged)
    Q_PROPERTY(QWifiNetworkListModel *networks READ networks CONSTANT)

public:
    enum NetworkState {
        Disconnected,
        ObtainingIPAddress,
        DhcpRequestFailed,
        Connected
    };

    QWifiManager();
    ~QWifiManager();

    QWifiNetworkListModel *networks() { return &m_networks; }
    QString connectedSSID() const { return m_connectedSSID; }
    bool scanning() const { return m_scanning; }
    void setScanning(bool scanning);
    NetworkState networkState() const { return m_state; }
    bool isbackendReady() const { return m_backendReady; }
    bool exitingEventThread() const { return m_exitingEventThread; }

public slots:
    void start();
    void stop();
    void connect(QWifiNetwork *network, const QString &passphrase);
    void disconnect();

signals:
    void scanningChanged(bool arg);
    void networkStateChanged();
    void backendReadyChanged();
    void connectedSSIDChanged(const QString &);

protected:
    bool event(QEvent *);
    void sendDhcpRequest(const QByteArray &request);
    void handleConnected();
    void connectToBackend();
    void disconnectFromBackend();
    QByteArray call(const char *command) const;
    bool checkedCall(const char *command) const;

protected slots:
    void connectedToDaemon();
    void handleDhcpReply();

private:
    friend class QWifiManagerEventThread;

    QString m_connectedSSID;
    QWifiNetworkListModel m_networks;
    QWifiManagerEventThread *m_eventThread;

    int m_scanTimer;
    bool m_scanning;
    bool m_backendReady;

    QByteArray m_interface;
    NetworkState m_state;
    QLocalSocket *m_daemonClientSocket;
    QByteArray m_request;
    bool m_exitingEventThread;
};

#endif // QWIFIMANAGER_H
