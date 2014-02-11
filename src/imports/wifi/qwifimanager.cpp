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
#include "qwifimanager.h"

#include <QtCore>

#include <hardware_legacy/wifi.h>
#include <cutils/sockets.h>
#include <unistd.h>

static const char SUPPLICANT_SVC[]  = "init.svc.wpa_supplicant";
static const char WIFI_INTERFACE[]  = "wifi.interface";
static const char QT_WIFI_BACKEND[] = "qt.wifi";

static bool QT_WIFI_DEBUG = !qgetenv("QT_WIFI_DEBUG").isEmpty();

const QEvent::Type WIFI_SCAN_RESULTS = (QEvent::Type) (QEvent::User + 2001);
const QEvent::Type WIFI_CONNECTED = (QEvent::Type) (QEvent::User + 2002);

/*
 * This function is borrowed from /system/core/libnetutils/dhcp_utils.c
 *
 * Wait for a system property to be assigned a specified value.
 * If desired_value is NULL, then just wait for the property to
 * be created with any value. maxwait is the maximum amount of
 * time in seconds to wait before giving up.
 */
static const int NAP_TIME = 200; // wait for 200ms at a time when polling for property values
static int wait_for_property(const char *name, const char *desired_value, int maxwait)
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

class QWifiManagerEventThread : public QThread
{
public:
    QWifiManagerEventThread(QWifiManager *manager, const QByteArray &interface)
        : m_manager(manager)
        , m_if(interface)
    {

    }

    void run() {
        if (QT_WIFI_DEBUG) qDebug("EventReceiver thread is running");
        char buffer[2048];
        while (1) {
            int size = wifi_wait_for_event(m_if.constData(), buffer, sizeof(buffer) - 1);
            if (size > 0) {
                buffer[size] = 0;

                if (QT_WIFI_DEBUG) qDebug("EVENT: %s", buffer);

                char *event = &buffer[11];
                if (strstr(event, "SCAN-RESULTS")) {
                    if (m_manager->exitingEventThread())
                        return;
                    QWifiManagerEvent *e = new QWifiManagerEvent(WIFI_SCAN_RESULTS);
                    QCoreApplication::postEvent(m_manager, e);
                } else if (strstr(event, "CONNECTED")) {
                    QWifiManagerEvent *e = new QWifiManagerEvent(WIFI_CONNECTED);
                    QCoreApplication::postEvent(m_manager, e);
                } else if (strstr(event, "TERMINATING")) {
                    // stop monitoring for events when supplicant is terminating
                    return;
                }
            }
        }
    }

    QWifiManager *m_manager;
    QByteArray m_if;
};

QWifiManager::QWifiManager()
    : m_networks(this)
    , m_eventThread(0)
    , m_scanTimer(0)
    , m_scanning(false)
    , m_daemonClientSocket(0)
    , m_exitingEventThread(false)
{
    char interface[PROPERTY_VALUE_MAX];
    property_get(WIFI_INTERFACE, interface, NULL);
    m_interface = interface;
    if (QT_WIFI_DEBUG) qDebug("QWifiManager: using wifi interface: %s", m_interface.constData());
    m_eventThread = new QWifiManagerEventThread(this, m_interface);

    m_daemonClientSocket = new QLocalSocket;
    int qconnFd = socket_local_client("qconnectivity", ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM);
    if (qconnFd != -1) {
        m_daemonClientSocket->setSocketDescriptor(qconnFd);
        QObject::connect(m_daemonClientSocket, SIGNAL(readyRead()), this, SLOT(handleDhcpReply()));
        QObject::connect(m_daemonClientSocket, SIGNAL(connected()), this, SLOT(connectedToDaemon()));
    } else {
        qWarning() << "QWifiManager: failed to connect to qconnectivity socket";
    }

    // check if backend has already been initialized
    char backend_status[PROPERTY_VALUE_MAX];
    if (property_get(QT_WIFI_BACKEND, backend_status, NULL)
            && strcmp(backend_status, "running") == 0) {
        // let it re-connect, in most cases this will see that everything is working properly
        // and will do nothing. Special case is when process has crashed or was killed by a system
        // signal in previous execution, which results in broken connection to a supplicant,
        // connectToBackend will fix it..
        connectToBackend();
    } else {
        // same here, cleans up the state
        disconnectFromBackend();
    }
}

QWifiManager::~QWifiManager()
{
    // exit event thread if it is running
    if (m_eventThread->isRunning()) {
        m_exitingEventThread = true;
        call("SCAN");
        m_eventThread->wait();
    }
    delete m_eventThread;
    delete m_daemonClientSocket;
}

void QWifiManager::handleDhcpReply()
{
    if (m_daemonClientSocket->canReadLine()) {
        QByteArray receivedMessage;
        receivedMessage = m_daemonClientSocket->readLine(m_daemonClientSocket->bytesAvailable());
        if (QT_WIFI_DEBUG) qDebug() << "QWifiManager: reply from qconnectivity: " << receivedMessage;
        if (receivedMessage == "success") {
            m_state = Connected;
            emit networkStateChanged();
            emit connectedSSIDChanged(m_connectedSSID);
            // Store settings of a working wifi connection
            call("SAVE_CONFIG");
        } else if (receivedMessage == "failed") {
            m_state = DhcpRequestFailed;
            emit networkStateChanged();
        } else {
            qWarning() << "QWifiManager: unknown message: " << receivedMessage;
        }
    }
}

void QWifiManager::sendDhcpRequest(const QByteArray &request)
{
    if (QT_WIFI_DEBUG) qDebug() << "QWifiManager: sending request - " << request;
    m_request = request;
    m_request.append("\n");
    m_daemonClientSocket->abort();
    // path where android stores "reserved" sockets
    m_daemonClientSocket->connectToServer(ANDROID_SOCKET_DIR "/qconnectivity");
}

void QWifiManager::connectedToDaemon()
{
    m_daemonClientSocket->write(m_request.constData(), m_request.length());
    m_daemonClientSocket->flush();
}

void QWifiManager::start()
{
    if (QT_WIFI_DEBUG) qDebug("QWifiManager: connecting to the backend");
    connectToBackend();
}

void QWifiManager::stop()
{
    if (QT_WIFI_DEBUG) qDebug("QWifiManager: shutting down");
    disconnectFromBackend();
}

void QWifiManager::connectToBackend()
{
    if (!(is_wifi_driver_loaded() || wifi_load_driver() == 0)) {
        qWarning("QWifiManager: failed to load a driver");
        return;
    }
    if (wifi_start_supplicant(0) != 0) {
        qWarning("QWifiManager: failed to start a supplicant");
        return;
    }
    if (wait_for_property(SUPPLICANT_SVC, "running", 5) < 0) {
        qWarning("QWifiManager: Timed out waiting for supplicant to start");
        return;
    }
    if (wifi_connect_to_supplicant(m_interface.constData()) == 0) {
        m_backendReady = true;
        emit backendReadyChanged();
        property_set(QT_WIFI_BACKEND, "running");
    } else {
        qWarning("QWifiManager: failed to connect to a supplicant");
        return;
    }
    if (QT_WIFI_DEBUG) qDebug("QWifiManager: started successfully");
    m_exitingEventThread = false;
    m_eventThread->start();
    handleConnected();
}

void QWifiManager::disconnectFromBackend()
{
    m_exitingEventThread = true;
    call("SCAN");
    m_eventThread->wait();

    if (wifi_stop_supplicant(0) < 0)
        qWarning("QWifiManager: failed to stop supplicant");
    wifi_close_supplicant_connection(m_interface.constData());
    property_set(QT_WIFI_BACKEND, "stopped");
    m_backendReady = false;
    emit backendReadyChanged();
}

void QWifiManager::setScanning(bool scanning)
{
    if (m_scanning == scanning)
        return;

    m_scanning = scanning;
    emit scanningChanged(m_scanning);

    if (m_scanning) {
        if (QT_WIFI_DEBUG) qDebug("QWifiManager: scanning");
        call("SCAN");
        m_scanTimer = startTimer(5000); // ### todo - this could be a qml property
    } else {
        if (QT_WIFI_DEBUG) qDebug("QWifiManager: stop scanning");
        killTimer(m_scanTimer);
    }
}

QByteArray QWifiManager::call(const char *command) const
{
    char data[2048];
    size_t len = sizeof(data) - 1;  // -1: room to add a 0-terminator
    if (wifi_command(m_interface.constData(), command, data, &len) < 0) {
        qWarning("QWifiManager: call failed: %s", command);
        return QByteArray();
    }
    if (len < sizeof(data))
        data[len] = 0;
    QByteArray result = QByteArray::fromRawData(data, len);
    if (QT_WIFI_DEBUG) qDebug("QWifiManager::call: %s ==>\n%s", command, result.constData());
    return result;
}

bool QWifiManager::checkedCall(const char *command) const
{
    return call(command).trimmed().toUpper() == "OK";
}

bool QWifiManager::event(QEvent *e)
{
    switch ((int) e->type()) {
    case WIFI_SCAN_RESULTS:
        m_networks.parseScanResults(call("SCAN_RESULTS"));
        return true;
    case WIFI_CONNECTED:
        handleConnected();
        break;
    case QEvent::Timer: {
        int tid = static_cast<QTimerEvent *>(e)->timerId();
        if (tid == m_scanTimer) {
            call("SCAN");
            return true;
        }
        break;
    }
    }

    return QObject::event(e);
}

void QWifiManager::connect(QWifiNetwork *network, const QString &passphrase)
{
    if (network->ssid() == m_connectedSSID) {
        if (QT_WIFI_DEBUG) qDebug("QWifiManager::connect(), already connected to %s", network->ssid().constData());
        return;
    }

    call("DISABLE_NETWORK all");
    if (!m_connectedSSID.isEmpty()) {
        m_connectedSSID.clear();
        emit connectedSSIDChanged(m_connectedSSID);
    }

    m_state = ObtainingIPAddress;
    emit networkStateChanged();
    bool networkKnown = false;
    QByteArray id;
    QByteArray listResult = call("LIST_NETWORKS");
    QList<QByteArray> lines = listResult.split('\n');
    foreach (const QByteArray &line, lines) {
        if (line.contains(network->ssid())) {
            int networkId = line.toInt();
            id = QByteArray::number(networkId);
            networkKnown = true;
            break;
        }
    }

    if (!networkKnown) {
        bool ok;
        QByteArray id = call("ADD_NETWORK").trimmed();
        id.toInt(&ok);
        if (!ok) {
            qWarning("QWifiManager::connect(), failed to add network");
            return;
        }
    }
    QByteArray setNetworkCommand = QByteArray("SET_NETWORK ") + id;

    bool ok = true;
    if (!networkKnown)
        ok = ok && checkedCall(setNetworkCommand + QByteArray(" ssid ") + '"' + network->ssid() + '"');

    QByteArray key_mgmt;
    if (network->supportsWPA() || network->supportsWPA2()) {
        ok = ok && checkedCall(setNetworkCommand + QByteArray(" psk ") + '"' + passphrase.toLatin1() + '"');
        key_mgmt = "WPA-PSK";
    } else if (network->supportsWEP()) {
        ok = ok && checkedCall(setNetworkCommand + QByteArray(" wep_key0 ") + '"' + passphrase.toLatin1() + '"');
        ok = ok && checkedCall(setNetworkCommand + QByteArray(" auth_alg OPEN SHARED"));
        key_mgmt = "NONE";
    } else if (!network->supportsWPS() && passphrase.length() == 0) {
        // open network
        key_mgmt = "NONE";
    }
    ok = ok && checkedCall(setNetworkCommand + QByteArray(" key_mgmt ") + key_mgmt);

    if (!ok) {
        if (!networkKnown)
            call("REMOVE_NETWORK " + id);
        qWarning("QWifiManager::connect(), failed to set properties on network '%s'", id.constData());
        return;
    }

    call(QByteArray("SELECT_NETWORK ") + id);
    call("RECONNECT");
}

void QWifiManager::disconnect()
{
    call("DISCONNECT");
    QByteArray req = m_interface;
    sendDhcpRequest(req.append(" disconnect"));
    m_state = Disconnected;
    m_connectedSSID.clear();
    emit networkStateChanged();
    emit connectedSSIDChanged(m_connectedSSID);
}

void QWifiManager::handleConnected()
{
    QList<QByteArray> lists = call("LIST_NETWORKS").split('\n');
    QByteArray connectedNetwork;
    for (int i=1; i<lists.size(); ++i) {
        if (lists.at(i).toUpper().contains("[CURRENT]")) {
            connectedNetwork = lists.at(i);
            break;
        }
    }

    if (connectedNetwork.isEmpty()) {
        if (QT_WIFI_DEBUG) qDebug("QWifiManager::handleConnected: not connected to a network...");
        m_state = Disconnected;
        emit networkStateChanged();
        return;
    }

    if (QT_WIFI_DEBUG) qDebug("QWifiManager::handleConnected: current is %s", connectedNetwork.constData());

    QList<QByteArray> info = connectedNetwork.split('\t');
    m_connectedSSID = info.at(1);

    QByteArray req = m_interface;
    sendDhcpRequest(req.append(" connect"));
}
