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
#ifndef QWIFICONTROLLER_H
#define QWIFICONTROLLER_H

#include "qwifimanager.h"

#include <QtCore/QEvent>
#include <QtCore/QVector>
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtCore/QLoggingCategory>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(B2QT_WIFI)

const QEvent::Type WIFI_SCAN_RESULTS = (QEvent::Type) (QEvent::User + 2001);
const QEvent::Type WIFI_CONNECTED = (QEvent::Type) (QEvent::User + 2002);
const QEvent::Type WIFI_HANDSHAKE_FAILED = (QEvent::Type) (QEvent::User + 2003);
const QEvent::Type WIFI_AUTHENTICATING = (QEvent::Type) (QEvent::User + 2004);
const QEvent::Type WIFI_DISCONNECTED = (QEvent::Type) (QEvent::User + 2005);

class QWifiManager;
class QWifiManagerPrivate;
class QWifiEventThread;
class QWifiSupplicant;

class QWifiEvent : public QEvent
{
public:
    QWifiEvent(QEvent::Type type, const QString &data = QString())
        : QEvent(type)
        , m_data(data)
    {
    }
    QString data() const { return m_data; }

private:
    QString m_data;
};

class QWifiController : public QThread
{
    Q_OBJECT
public:
    enum Method {
        InitializeBackend,
        TerminateBackend,
        AcquireIPAddress,
        StopDhcp,
        ExitEventLoop
    };

    explicit QWifiController(QWifiManager *manager, QWifiManagerPrivate *managerPrivate);
    ~QWifiController();

    void asyncCall(Method method);
    QWifiManager *wifiManager() const { return m_manager; }
    bool isWifiThreadExitRequested() const { return m_exitEventThread; }
    void startWifiEventThread();
    void acquireIPAddress();
    void stopDhcp() const;
    bool resetSupplicantSocket();
    QWifiSupplicant *supplicant() const { return m_supplicant; }

signals:
    void backendStateChanged(QWifiManager::BackendState backendState);
    void dhcpRequestFinished(const QString &status);

protected:
    void run();
    void initializeBackend();
    void terminateBackend();
    void exitWifiEventThread();
    void killDhcpProcess(const QString &path) const;

private:
    QWifiManager *m_manager;
    QWifiManagerPrivate *const m_managerPrivate;
    bool m_exitEventThread;
    QByteArray m_interface;
    QVector<Method> m_methods;
    QWifiEventThread *m_eventThread;
    QMutex m_methodsMutex;
    QWaitCondition methodCallRequested;
    QWifiSupplicant *m_supplicant;
};

QT_END_NAMESPACE

#endif // QWIFICONTROLLER_H
