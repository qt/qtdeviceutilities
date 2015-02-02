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

#ifdef Q_OS_ANDROID_NO_SDK
#include <QtNetwork/QLocalSocket>
#include <cutils/sockets.h>
#include <unistd.h>
#endif

#ifdef Q_OS_ANDROID_NO_SDK
/*
 * Work API differences between Android versions
 */
int q_wifi_start_supplicant()
{
#if Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR < 1
    return wifi_start_supplicant();
#else
    return wifi_start_supplicant(0);
#endif
}

int q_wifi_stop_supplicant()
{
#if Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR < 1
    return wifi_stop_supplicant();
#else
    return wifi_stop_supplicant(0);
#endif
}

int q_wifi_connect_to_supplicant(const char *ifname)
{
#if Q_ANDROID_VERSION_MAJOR == 4 && (Q_ANDROID_VERSION_MINOR < 4 && Q_ANDROID_VERSION_MINOR >= 1)
    return wifi_connect_to_supplicant(ifname);
#else
    Q_UNUSED(ifname);
    return wifi_connect_to_supplicant();
#endif
}

void q_wifi_close_supplicant_connection(const char *ifname)
{
#if Q_ANDROID_VERSION_MAJOR == 4 && (Q_ANDROID_VERSION_MINOR < 4 && Q_ANDROID_VERSION_MINOR >= 1)
    wifi_close_supplicant_connection(ifname);
#else
    Q_UNUSED(ifname);
    wifi_close_supplicant_connection();
#endif
}

int q_wifi_wait_for_event(const char *ifname, char *buf, size_t len)
{
#if Q_ANDROID_VERSION_MAJOR == 4 && (Q_ANDROID_VERSION_MINOR < 4 && Q_ANDROID_VERSION_MINOR >= 1)
    return wifi_wait_for_event(ifname, buf, len);
#else
    Q_UNUSED(ifname);
    return wifi_wait_for_event(buf, len);
#endif
}

int q_wifi_command(const char *ifname, const char *command, char *reply, size_t *reply_len)
{
#if Q_ANDROID_VERSION_MAJOR == 4 && (Q_ANDROID_VERSION_MINOR < 4 && Q_ANDROID_VERSION_MINOR >= 1)
    return wifi_command(ifname, command, reply, reply_len);
#else
    Q_UNUSED(ifname);
    return wifi_command(command, reply, reply_len);
#endif
}

/*
 * This function is borrowed from /system/core/libnetutils/dhcp_utils.c
 *
 * Wait for a system property to be assigned a specified value.
 * If desired_value is NULL, then just wait for the property to
 * be created with any value. maxwait is the maximum amount of
 * time in seconds to wait before giving up.
 */
const int NAP_TIME = 200; // wait for 200ms at a time when polling for property values
int wait_for_property(const char *name, const char *desired_value, int maxwait)
{
    char value[PROPERTY_VALUE_MAX] = {'\0'};
    int maxnaps = (maxwait * 1000) / NAP_TIME;

    if (maxnaps < 1) {
        maxnaps = 1;
    }

    while (maxnaps-- > 0) {
        usleep(NAP_TIME * 1000);
        if (property_get(name, value, NULL)) {
            if (desired_value == NULL ||
                    strcmp(value, desired_value) == 0) {
                return 0;
            }
        }
    }
    return -1; /* failure */
}

#endif // Q_OS_ANDROID_NO_SDK

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(B2QT_WIFI, "qt.b2qt.wifi")

class QWifiManagerEvent : public QEvent
{
public:
    QWifiManagerEvent(QEvent::Type type, const QByteArray &data = QByteArray())
        : QEvent(type)
        , m_data(data)
    {
    }

    QByteArray data() const { return m_data; }

private:
    QByteArray m_data;
};

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
        QWifiManagerEvent *event = 0;
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
                    event = new QWifiManagerEvent(WIFI_SCAN_RESULTS);
                } else if (strstr(buffer, "CTRL-EVENT-CONNECTED")) {
                    event = new QWifiManagerEvent(WIFI_CONNECTED);
                } else if (strstr(buffer, "CTRL-EVENT-DISCONNECTED")) {
                    event = new QWifiManagerEvent(WIFI_DISCONNECTED);
                } else if (strstr(buffer, "Trying to associate")) {
                    event = new QWifiManagerEvent(WIFI_AUTHENTICATING);
                } else if (strstr(buffer, "Handshake failed")) {
                    event = new QWifiManagerEvent(WIFI_HANDSHAKE_FAILED);
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
#ifdef Q_OS_ANDROID_NO_SDK
    m_qcSocket(0),
#endif
    m_eventThread(0)
{
    m_interface = QWifiDevice::wifiInterfaceName();
    m_eventThread = new QWifiEventThread(this);

    qRegisterMetaType<QWifiManager::BackendState>("QWifiManager::BackendState");
#ifdef Q_OS_ANDROID_NO_SDK
    qRegisterMetaType<QAbstractSocket::SocketState>("QAbstractSocket::SocketState");
#endif
}

QWifiController::~QWifiController()
{
    exitWifiEventThread();
    delete m_eventThread;
#ifdef Q_OS_ANDROID_NO_SDK
    delete m_qcSocket;
#endif
}

void QWifiController::allocateOnThisThread()
{
#ifdef Q_OS_ANDROID_NO_SDK
    m_qcSocket = new QLocalSocket;
    int qcFd = socket_local_client("qconnectivity", ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM);
    if (qcFd != -1) {
        m_qcSocket->setSocketDescriptor(qcFd);
    } else {
        qCWarning(B2QT_WIFI) << "failed to get file descriptor of a qconnectivity socket!";
    }
#endif
}

void QWifiController::run()
{
    qCDebug(B2QT_WIFI) << "running wifi backend controller thread";
    allocateOnThisThread();
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
    bool initFailed = false;
#ifdef Q_OS_ANDROID_NO_SDK
    qCDebug(B2QT_WIFI) << "initialize driver";
    if (!(is_wifi_driver_loaded() || wifi_load_driver() == 0)) {
        qCWarning(B2QT_WIFI) << "failed to load a driver";
        initFailed = true;
    }
#else
    qCDebug(B2QT_WIFI) << "run ifconfig (up)";
    QProcess ifconfig;
    ifconfig.start(QStringLiteral("ifconfig"), QStringList() << QLatin1String(m_interface) << QStringLiteral("up"));
    ifconfig.waitForFinished();
    if (ifconfig.exitStatus() != QProcess::NormalExit && ifconfig.exitCode() != 0) {
        qCWarning(B2QT_WIFI) << "failed to bring up wifi interface!";
        initFailed = true;
    }
#endif
    if (!initFailed && resetSupplicantSocket()) {
        startWifiEventThread();
        qCDebug(B2QT_WIFI) << "wifi backend started successfully";
        emit backendStateChanged(QWifiManager::Running);
    } else {
        emit backendStateChanged(QWifiManager::NotRunning);
    }
}

bool QWifiController::resetSupplicantSocket() const
{
    qCDebug(B2QT_WIFI) << "reset supplicant socket";
    // close down the previous connection to supplicant if
    // one exists before re-connecting.
    if (q_wifi_stop_supplicant() < 0)
        qCWarning(B2QT_WIFI) << "failed to stop supplicant!";
    q_wifi_close_supplicant_connection(m_interface);
    qCDebug(B2QT_WIFI) << "start supplicant";
    if (q_wifi_start_supplicant() != 0) {
        qCWarning(B2QT_WIFI) << "failed to start a supplicant!";
        return false;
    }
#ifdef Q_OS_ANDROID_NO_SDK
    if (wait_for_property("init.svc.wpa_supplicant", "running", 5) < 0) {
        qCWarning(B2QT_WIFI) << "timed out waiting for supplicant to start!";
        return false;
    }
#endif
    qCDebug(B2QT_WIFI) << "connect to supplicant";
    if (q_wifi_connect_to_supplicant(m_interface) != 0) {
        qCWarning(B2QT_WIFI) << "failed to connect to a supplicant!";
        return false;
    }
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
#ifndef Q_OS_ANDROID_NO_SDK
    qCDebug(B2QT_WIFI) << "run ifconfig (down)";
    QProcess ifconfig;
    ifconfig.start(QStringLiteral("ifconfig"), QStringList() << QLatin1String(m_interface) << QStringLiteral("down"));
    ifconfig.waitForFinished();
    if (ifconfig.exitStatus() != QProcess::NormalExit && ifconfig.exitCode() != 0)
        qCWarning(B2QT_WIFI) << "failed to bring down wifi interface!";
#endif
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
        m_eventThread->wait();
    }
}

#ifdef Q_OS_ANDROID_NO_SDK
bool QWifiController::getQConnectivityReply()
{
    bool arrived = false;
    if (m_qcSocket->canReadLine()) {
        arrived = true;
        QByteArray received = m_qcSocket->readLine(m_qcSocket->bytesAvailable());
        if (received != "success" && received != "failed") {
            qCWarning(B2QT_WIFI) << "unknown message: " << received;
            received = "failed";
        }
        emit dhcpRequestFinished(QLatin1String(received));
    }
    return arrived;
}
#else
void QWifiController::killDhcpProcess(const QString &path) const
{
    QFile pidFile(path);
    if (pidFile.exists()) {
        pidFile.open(QIODevice::ReadOnly);
        QByteArray pid = pidFile.readAll();
        QProcess kill;
        kill.start(QStringLiteral("kill"), QStringList() << QLatin1String(pid.trimmed()));
        kill.waitForFinished();
        if (kill.exitStatus() != QProcess::NormalExit && kill.exitCode() != 0)
            qCWarning(B2QT_WIFI) << "killing dhcp process failed!";
    }
    pidFile.close();
}
#endif

void QWifiController::acquireIPAddress()
{
    qCDebug(B2QT_WIFI, "acquireIPAddress");
#ifdef Q_OS_ANDROID_NO_SDK
    QByteArray request = m_interface + " connect\n";
    m_qcSocket->abort();
    m_qcSocket->connectToServer(QStringLiteral(ANDROID_SOCKET_DIR "/qconnectivity"));
    bool timeout = false;
    if (m_qcSocket->waitForConnected()) {
        m_qcSocket->write(request, request.length());
        m_qcSocket->flush();
        do {
            if (m_qcSocket->waitForReadyRead()) {
                if (getQConnectivityReply())
                    break;
            } else {
                timeout = true;
                qCWarning(B2QT_WIFI) << "waiting a message from qconnectivity timed out!";
                break;
            }
        } while (true);
    } else {
        timeout = true;
        qCWarning(B2QT_WIFI) << "connecting to qconnectivity server socket timed out!";
    }
    if (timeout)
        emit dhcpRequestFinished(QStringLiteral("failed"));
#else
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
#endif
}

void QWifiController::stopDhcp() const
{
    qCDebug(B2QT_WIFI, "stopDhcp");
#ifdef Q_OS_ANDROID_NO_SDK
    QByteArray request = m_interface + " disconnect\n";
    m_qcSocket->abort();
    m_qcSocket->connectToServer(QStringLiteral(ANDROID_SOCKET_DIR "/qconnectivity"));
    if (m_qcSocket->waitForConnected()) {
        m_qcSocket->write(request, request.length());
        m_qcSocket->flush();
    } else {
        qCWarning(B2QT_WIFI) << "connecting to qconnectivity server socket timed out!";
    }
#else
    QString filePath = QLatin1String("/var/run/udhcpc." + m_interface + ".pid");
    killDhcpProcess(filePath);
#endif
}

QT_END_NAMESPACE
