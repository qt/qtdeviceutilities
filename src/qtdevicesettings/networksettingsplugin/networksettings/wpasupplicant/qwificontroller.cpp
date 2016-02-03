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
#include "qnetworksettingsmanager_p.h"
#include "qwifisupplicant_p.h"
#include "qwifidevice.h"

#include <sys/types.h>
#include <signal.h>

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
    }

    void run() {
        qCDebug(B2QT_WIFI) << "running wifi event thread";
        QWifiEvent *event = 0;
        char buffer[2048];
        forever {
            int size = m_controller->supplicant()->waitForEvent(buffer, sizeof(buffer) - 1);
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

                if (event) {
                    QCoreApplication::postEvent(m_controller->manager(), event);
                }
            }
        }
    }

private:
    QWifiController *m_controller;
};


QWifiController::QWifiController(QNetworkSettingsManagerPrivate *manager) :
    m_manager(manager),
    m_exitEventThread(false),
    m_interface(QWifiDevice::wifiInterfaceName()),
    m_eventThread(new QWifiEventThread(this)),
    m_supplicant(new QWifiSupplicant(this))
{
    qRegisterMetaType<QWifiController::BackendState>("BackendState");
    connect(m_supplicant, &QWifiSupplicant::raiseError, this, &QWifiController::raiseError);
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

void QWifiController::asyncCall(Method method)
{
    QMutexLocker locker(&m_methodsMutex);
    m_methods.append(method);
    methodCallRequested.wakeOne();
}

void QWifiController::initializeBackend()
{
    qCDebug(B2QT_WIFI) << "initializing wifi backend";
    emit backendStateChanged(QWifiController::Initializing);

    QProcess rfkill;
    rfkill.start(QStringLiteral("rfkill"),
                 QStringList() << QStringLiteral("unblock") << QStringLiteral("wifi"));
    rfkill.waitForFinished();

    QProcess ifconfig;
    ifconfig.start(QStringLiteral("ifconfig"),
                   QStringList() << QLatin1String(m_interface) << QStringLiteral("up"));
    if (!ifconfig.waitForStarted()) {
        emit raiseError(ifconfig.program() + QLatin1String(": ") + ifconfig.errorString());
        return;
    }

    ifconfig.waitForFinished();
    bool initFailed = false;
    QByteArray error = ifconfig.readAllStandardError();
    if (!error.isEmpty()) {
        emit raiseError(QLatin1String("failed to bring up wifi interface: " + error));
        initFailed = true;
    }
    if (!initFailed && resetSupplicantSocket())
        emit backendStateChanged(QWifiController::Running);
    else
        emit backendStateChanged(QWifiController::NotRunning);
}

bool QWifiController::resetSupplicantSocket()
{
    qCDebug(B2QT_WIFI) << "reset supplicant socket";
    exitWifiEventThread();
    m_supplicant->stopSupplicant();
    m_supplicant->closeSupplicantConnection();
    if (!m_supplicant->startSupplicant())
        return false;
    if (!m_supplicant->connectToSupplicant())
        return false;

    startWifiEventThread();
    return true;
}

void QWifiController::terminateBackend()
{
    qCDebug(B2QT_WIFI) << "terminating wifi backend";
    emit backendStateChanged(QWifiController::Terminating);

    exitWifiEventThread();
    m_supplicant->stopSupplicant();
    m_supplicant->closeSupplicantConnection();

    QProcess ifconfig;
    ifconfig.start(QStringLiteral("ifconfig"),
                   QStringList() << QLatin1String(m_interface) << QStringLiteral("down"));
    if (!ifconfig.waitForStarted()) {
        emit raiseError(ifconfig.program() + QLatin1String(": ") + ifconfig.errorString());
        return;
    }

    ifconfig.waitForFinished();
    QByteArray error = ifconfig.readAllStandardError();
    if (!error.isEmpty())
        emit raiseError(QLatin1String("failed to bring down wifi interface: " + error));

    stopDhcp();
    emit backendStateChanged(QWifiController::NotRunning);
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
        m_manager->call(QStringLiteral("SCAN"));
        if (!m_eventThread->wait(8000))
            qCWarning(B2QT_WIFI, "timed out waiting for wifi event thread to exit");
    }
}

void QWifiController::killDhcpProcess(const QString &path) const
{
    QFile pidFile(path);
    if (!pidFile.exists())
        return;

    if (!pidFile.open(QIODevice::ReadOnly)) {
        qCWarning(B2QT_WIFI) << "could not open pid file: " << path;
        return;
    }

    bool ok;
    int pid = pidFile.readAll().trimmed().toInt(&ok);
    if (!ok) {
        qCWarning(B2QT_WIFI) << "pid is not a number";
        return;
    }

    kill(pid, 9);
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
    if (!udhcpc.waitForStarted()) {
        emit raiseError(udhcpc.program() + QLatin1String(": ") + udhcpc.errorString());
        emit dhcpRequestFinished(QLatin1String("failed"));
        return;
    }

    udhcpc.waitForFinished();
    QByteArray error = udhcpc.readAllStandardError();
    QString status = QLatin1String("success");
    if (!error.isEmpty()) {
        emit raiseError(QLatin1String("udhcpc failed: " + error));
        status = QLatin1String("failed");
    } else {
        if (udhcpc.readAllStandardOutput().contains("No lease"))
            status = QLatin1String("failed");
    }

    emit dhcpRequestFinished(status);
}

void QWifiController::stopDhcp() const
{
    qCDebug(B2QT_WIFI, "stopDhcp");
    QString filePath = QLatin1String("/var/run/udhcpc." + m_interface + ".pid");
    killDhcpProcess(filePath);
}

QT_END_NAMESPACE
