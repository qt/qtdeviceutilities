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
#include "qwificontroller_p.h"
#include "qwifimanager_p.h"
#include "qwifidevice.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QProcess>
#include <QtCore/QByteArray>
#include <QtCore/QFile>

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(B2QT_WIFI, "qt.b2qt.wifi")

class QWifiEventThread : public QThread
{
public:
    QWifiEventThread(QWifiController *controller)
        : m_controller(controller)
    {
        m_interface = QWifiDevice::wifiInterfaceName();
    }

    void run() {
        qCDebug(B2QT_WIFI) << "running wifi event thread";
        QWifiEvent *event = 0;
        char buffer[2048];
        forever {
            int size = q_wifi_wait_for_event(m_interface, buffer, sizeof(buffer) - 1);
            if (size > 0) {
                buffer[size] = 0;
                event = 0;
                qCDebug(B2QT_WIFI) << "[event]: " << buffer;

                if (m_controller->isWifiThreadExitRequested()) {
                    qCDebug(B2QT_WIFI) << "exit wifi event thread";
                    return;
                }

                if (strstr(buffer, "SCAN-RESULTS")) {
                    event = new QWifiEvent(WIFI_SCAN_RESULTS);
                } else if (strstr(buffer, "CTRL-EVENT-CONNECTED")) {
                    event = new QWifiEvent(WIFI_CONNECTED);
                } else if (strstr(buffer, "CTRL-EVENT-DISCONNECTED")) {
                    event = new QWifiEvent(WIFI_DISCONNECTED);
                } else if (strstr(buffer, "Trying to associate")) {
                    event = new QWifiEvent(WIFI_AUTHENTICATING, QLatin1String(buffer));
                } else if (strstr(buffer, "Handshake failed")) {
                    event = new QWifiEvent(WIFI_HANDSHAKE_FAILED);
                }

                if (event)
                    QCoreApplication::postEvent(m_controller->wifiManager(), event);
            }
        }
    }

private:
    QWifiController *m_controller;
    QByteArray m_interface;
};


QWifiController::QWifiController(QWifiManager *manager, QWifiManagerPrivate *managerPrivate) :
    m_manager(manager),
    m_managerPrivate(managerPrivate),
    m_exitEventThread(false),
    m_eventThread(0)
{
    m_interface = QWifiDevice::wifiInterfaceName();
    m_eventThread = new QWifiEventThread(this);

    qRegisterMetaType<QWifiManager::BackendState>("QWifiManager::BackendState");
}

QWifiController::~QWifiController()
{
    exitWifiEventThread();
    delete m_eventThread;
}

void QWifiController::run()
{
    qCDebug(B2QT_WIFI) << "running wifi backend controller thread";
    Method method;
    forever {
        m_methodsMutex.lock();
        if (m_methods.isEmpty())
            methodCallRequested.wait(&m_methodsMutex);
        method = m_methods.takeFirst();
        m_methodsMutex.unlock();
        switch (method) {
        case InitializeBackend:
            initializeBackend();
            break;
        case TerminateBackend:
            terminateBackend();
            break;
        case AcquireIPAddress:
            acquireIPAddress();
            break;
        case StopDhcp:
            stopDhcp();
            break;
        case ExitEventLoop:
            qCDebug(B2QT_WIFI) << "exit wifi backend controller thread";
            return;
        }
    }
}

void QWifiController::call(Method method)
{
    QMutexLocker locker(&m_methodsMutex);
    m_methods.append(method);
    methodCallRequested.wakeOne();
}

void QWifiController::initializeBackend()
{
    qCDebug(B2QT_WIFI) << "initializing wifi backend";
    emit backendStateChanged(QWifiManager::Initializing);
    qCDebug(B2QT_WIFI) << "run ifconfig (up)";
    QProcess ifconfig;
    ifconfig.start(QStringLiteral("ifconfig"), QStringList() << QLatin1String(m_interface) << QStringLiteral("up"));
    ifconfig.waitForFinished();
    bool initFailed = false;
    if (ifconfig.exitStatus() != QProcess::NormalExit && ifconfig.exitCode() != 0) {
        qCWarning(B2QT_WIFI) << "failed to bring up wifi interface!";
        initFailed = true;
    }
    if (!initFailed && resetSupplicantSocket()) {
        qCDebug(B2QT_WIFI) << "wifi backend started successfully";
        emit backendStateChanged(QWifiManager::Running);
    } else {
        emit backendStateChanged(QWifiManager::NotRunning);
    }
}

bool QWifiController::resetSupplicantSocket()
{
    qCDebug(B2QT_WIFI) << "reset supplicant socket";
    exitWifiEventThread();
    if (q_wifi_stop_supplicant() < 0)
        qCWarning(B2QT_WIFI) << "failed to stop supplicant!";
    q_wifi_close_supplicant_connection(m_interface);
    qCDebug(B2QT_WIFI) << "start supplicant";
    if (q_wifi_start_supplicant() != 0) {
        qCWarning(B2QT_WIFI) << "failed to start a supplicant!";
        return false;
    }
    qCDebug(B2QT_WIFI) << "connect to supplicant";
    if (q_wifi_connect_to_supplicant(m_interface) != 0) {
        qCWarning(B2QT_WIFI) << "failed to connect to a supplicant!";
        return false;
    }
    startWifiEventThread();
    return true;
}

void QWifiController::terminateBackend()
{
    qCDebug(B2QT_WIFI) << "terminating wifi backend";
    emit backendStateChanged(QWifiManager::Terminating);
    exitWifiEventThread();
    if (q_wifi_stop_supplicant() < 0)
        qCWarning(B2QT_WIFI) << "failed to stop supplicant!";
    q_wifi_close_supplicant_connection(m_interface);

    qCDebug(B2QT_WIFI) << "run ifconfig (down)";
    QProcess ifconfig;
    ifconfig.start(QStringLiteral("ifconfig"), QStringList() << QLatin1String(m_interface) << QStringLiteral("down"));
    ifconfig.waitForFinished();
    if (ifconfig.exitStatus() != QProcess::NormalExit && ifconfig.exitCode() != 0)
        qCWarning(B2QT_WIFI) << "failed to bring down wifi interface!";

    stopDhcp();
    qCDebug(B2QT_WIFI) << "wifi backend stopped successfully";
    emit backendStateChanged(QWifiManager::NotRunning);
}

void QWifiController::startWifiEventThread()
{
    m_exitEventThread = false;
    m_eventThread->start();
}

void QWifiController::exitWifiEventThread()
{
    if (m_eventThread->isRunning()) {
        m_exitEventThread = true;
        m_managerPrivate->call(QStringLiteral("SCAN"));
        if (!m_eventThread->wait(8000))
            qCWarning(B2QT_WIFI, "timed out waiting for wifi event thread to exit!");
    }
}

void QWifiController::killDhcpProcess(const QString &path) const
{
    QFile pidFile(path);
    if (!pidFile.exists() || !pidFile.open(QIODevice::ReadOnly)) {
        qCWarning(B2QT_WIFI) << "could not read pid file: " << path;
        return;
    }

    QByteArray pid = pidFile.readAll();
    QProcess kill;
    kill.start(QStringLiteral("kill"), QStringList() << QStringLiteral("-9") << QLatin1String(pid.trimmed()));
    kill.waitForFinished();
    if (kill.exitStatus() != QProcess::NormalExit && kill.exitCode() != 0)
        qCWarning(B2QT_WIFI) << "killing dhcp process failed!";
}

void QWifiController::acquireIPAddress()
{
    qCDebug(B2QT_WIFI, "acquireIPAddress");
    QString filePath = QLatin1String("/var/run/udhcpc." + m_interface + ".pid");
    killDhcpProcess(filePath);
    QStringList args;
    args << QStringLiteral("-R") << QStringLiteral("-n") << QStringLiteral("-p")
         << filePath << QStringLiteral("-i") << QLatin1String(m_interface);

    QProcess udhcpc;
    udhcpc.start(QStringLiteral("udhcpc"), args);
    udhcpc.waitForFinished();
    if (udhcpc.exitStatus() != QProcess::NormalExit && udhcpc.exitCode() != 0)
        qCWarning(B2QT_WIFI) << "udhcpc process failed!";

    QString status = QLatin1String("success");
    if (udhcpc.readAll().contains("No lease"))
        status = QLatin1String("failed");

    emit dhcpRequestFinished(status);
}

void QWifiController::stopDhcp() const
{
    qCDebug(B2QT_WIFI, "stopDhcp");
    QString filePath = QLatin1String("/var/run/udhcpc." + m_interface + ".pid");
    killDhcpProcess(filePath);
}

QT_END_NAMESPACE
