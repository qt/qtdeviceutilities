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

#ifdef Q_OS_ANDROID_NO_SDK
#include <hardware_legacy/wifi.h>
#include <cutils/properties.h>
#else
#include "qwifielinux_p.h"
#endif

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(B2QT_WIFI)

#ifdef Q_OS_ANDROID_NO_SDK
int q_wifi_start_supplicant();
int q_wifi_stop_supplicant();
int q_wifi_connect_to_supplicant(const char *ifname);
void q_wifi_close_supplicant_connection(const char *ifname);
int q_wifi_wait_for_event(const char *ifname, char *buf, size_t len);
int q_wifi_command(const char *ifname, const char *command, char *reply, size_t *reply_len);
int wait_for_property(const char *name, const char *desired_value, int maxwait);
#endif

const QEvent::Type WIFI_SCAN_RESULTS = (QEvent::Type) (QEvent::User + 2001);
const QEvent::Type WIFI_CONNECTED = (QEvent::Type) (QEvent::User + 2002);
const QEvent::Type WIFI_HANDSHAKE_FAILED = (QEvent::Type) (QEvent::User + 2003);
const QEvent::Type WIFI_AUTHENTICATING = (QEvent::Type) (QEvent::User + 2004);
const QEvent::Type WIFI_DISCONNECTED = (QEvent::Type) (QEvent::User + 2005);

class QWifiManager;
class QWifiManagerPrivate;
class QWifiEventThread;
#ifdef Q_OS_ANDROID_NO_SDK
class QLocalSocket;
#endif

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

    void call(Method method);
    QWifiManager *wifiManager() const { return m_manager; }
    bool isWifiThreadExitRequested() const { return m_exitEventThread; }
    void startWifiEventThread();
    void acquireIPAddress();
    void stopDhcp() const;
    void resetSupplicantSocket() const;

signals:
    void backendStateChanged(QWifiManager::BackendState backendState);
    void dhcpRequestFinished(const QString &status);

protected:
    void run();
    void initializeBackend();
    void terminateBackend();
    void exitWifiEventThread();
    void allocateOnThisThread();
#ifdef Q_OS_ANDROID_NO_SDK
    bool getQConnectivityReply();
#else
    void killDhcpProcess(const QString &path) const;
#endif

private:
    QWifiManager *m_manager;
    QWifiManagerPrivate *const m_managerPrivate;
    bool m_exitEventThread;
    QByteArray m_interface;
#ifdef Q_OS_ANDROID_NO_SDK
    QLocalSocket *m_qcSocket;
#endif
    QVector<Method> m_methods;
    QWifiEventThread *m_eventThread;
    QMutex m_methodsMutex;
    QWaitCondition methodCallRequested;
};

QT_END_NAMESPACE

#endif // QWIFICONTROLLER_H
