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
#include <QtCore/QThread>
#include <QtCore/QByteArray>

#ifdef Q_OS_ANDROID
#include <QtNetwork/QLocalSocket>
#include <cutils/properties.h>
#endif

#include "qwifinetworklistmodel.h"

class QWifiManagerEventThread;

class ProcessRunner : public QThread
{
    Q_OBJECT
public:
    ProcessRunner(const QByteArray &ifc) : m_ifc(ifc) {}
    void run();

signals:
    void processFinished();

private:
    QByteArray m_ifc;
};

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
        Authenticating,
        HandshakeFailed,
        ObtainingIPAddress,
        DhcpRequestFailed,
        Connected
    };

    QWifiManager();
    ~QWifiManager();

    QWifiNetworkListModel *networks() { return &m_networkListModel; }
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
    void scanningChanged(bool scanning);
    void networkStateChanged(QWifiNetwork *network);
    void backendReadyChanged();
    void connectedSSIDChanged(const QString &ssid);

protected:
    bool event(QEvent *);
    void handleConnected();
    void connectToBackend();
    void disconnectFromBackend();
    void exitEventThread();

    QByteArray call(const char *command) const;
    bool checkedCall(const char *command) const;
    void updateNetworkState(NetworkState state);

protected slots:
#if defined(FORCE_MOC)
    void sendDhcpRequest(const QByteArray &request);
    void connectedToDaemon();
    void handleDhcpReply();
#endif
    void handleDhcpFinished();

private:
    friend class QWifiManagerEventThread;
    friend class ProcessRunner;

    QString m_connectedSSID;
    QWifiNetworkListModel m_networkListModel;
    QWifiManagerEventThread *m_eventThread;

    int m_scanTimer;
    bool m_scanning;
    bool m_backendReady;

    QByteArray m_interface;
    NetworkState m_state;
#ifdef Q_OS_ANDROID
    QLocalSocket *m_daemonClientSocket;
#else
    ProcessRunner *m_dhcpRunner;
#endif
    QByteArray m_request;
    bool m_exitingEventThread;
    bool m_startingUp;
    QWifiNetwork *m_network;
};

#endif // QWIFIMANAGER_H
