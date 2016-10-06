/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Device Utilities module of the Qt Toolkit.
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
#ifndef QWIFICONTROLLER_H
#define QWIFICONTROLLER_H

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

class QWifiEventThread;
class QWifiSupplicant;
class QNetworkSettingsManagerPrivate;

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
    Q_ENUMS(BackendState)
public:
    enum Method {
        InitializeBackend,
        TerminateBackend,
        AcquireIPAddress,
        StopDhcp,
        ExitEventLoop
    };

    enum BackendState {
        Initializing,
        Running,
        Terminating,
        NotRunning
    };

    explicit QWifiController(QNetworkSettingsManagerPrivate *manager);
    ~QWifiController();

    void asyncCall(Method method);
    QNetworkSettingsManagerPrivate *manager() const { return m_manager; }
    bool isWifiThreadExitRequested() const { return m_exitEventThread; }
    void startWifiEventThread();
    void acquireIPAddress();
    void stopDhcp() const;
    bool resetSupplicantSocket();
    QWifiSupplicant *supplicant() const { return m_supplicant; }

Q_SIGNALS:
    void backendStateChanged(BackendState backendState);
    void dhcpRequestFinished(const QString &status);
    void raiseError(const QString &error);

protected:
    void run();
    void initializeBackend();
    void terminateBackend();
    void exitWifiEventThread();
    void killDhcpProcess(const QString &path) const;

private:
    QNetworkSettingsManagerPrivate *m_manager; //not owned
    bool m_exitEventThread;
    QByteArray m_interface;
    QVector<Method> m_methods;
    QWifiEventThread *m_eventThread;
    QMutex m_methodsMutex;
    QWaitCondition methodCallRequested;
    QWifiSupplicant *m_supplicant;
};

Q_DECLARE_METATYPE(QWifiController::BackendState)

QT_END_NAMESPACE

#endif // QWIFICONTROLLER_H
