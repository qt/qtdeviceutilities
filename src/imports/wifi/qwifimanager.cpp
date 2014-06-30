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
#ifdef Q_OS_ANDROID
#include <hardware_legacy/wifi.h>
#include <cutils/sockets.h>
#include <unistd.h>
#else
#include <qwifi_elinux.h>
#endif

static const char SUPPLICANT_SVC[]  = "init.svc.wpa_supplicant";
static const char WIFI_INTERFACE[]  = "wifi.interface";
static const char QT_WIFI_BACKEND[] = "qt.wifi";

static bool QT_WIFI_DEBUG = !qgetenv("QT_WIFI_DEBUG").isEmpty();

const QEvent::Type WIFI_SCAN_RESULTS = (QEvent::Type) (QEvent::User + 2001);
const QEvent::Type WIFI_CONNECTED = (QEvent::Type) (QEvent::User + 2002);
const QEvent::Type WIFI_HANDSHAKE_FAILED = (QEvent::Type) (QEvent::User + 2003);

#ifdef Q_OS_ANDROID
/*
 * Work around API differences between Android versions
 */

static int q_wifi_start_supplicant()
{
#if Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR < 1
    return wifi_start_supplicant();
#else
    return wifi_start_supplicant(0);
#endif
}

static int q_wifi_stop_supplicant()
{
#if Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR < 1
    return wifi_stop_supplicant();
#else
    return wifi_stop_supplicant(0);
#endif
}

static int q_wifi_connect_to_supplicant(const char *ifname)
{
#if Q_ANDROID_VERSION_MAJOR == 4 && (Q_ANDROID_VERSION_MINOR < 4 && Q_ANDROID_VERSION_MINOR >= 1)
    return wifi_connect_to_supplicant(ifname);
#else
    Q_UNUSED(ifname);
    return wifi_connect_to_supplicant();
#endif
}

static void q_wifi_close_supplicant_connection(const char *ifname)
{
#if Q_ANDROID_VERSION_MAJOR == 4 && (Q_ANDROID_VERSION_MINOR < 4 && Q_ANDROID_VERSION_MINOR >= 1)
    wifi_close_supplicant_connection(ifname);
#else
    Q_UNUSED(ifname);
    wifi_close_supplicant_connection();
#endif
}

static int q_wifi_wait_for_event(const char *ifname, char *buf, size_t len)
{
#if Q_ANDROID_VERSION_MAJOR == 4 && (Q_ANDROID_VERSION_MINOR < 4 && Q_ANDROID_VERSION_MINOR >= 1)
    return wifi_wait_for_event(ifname, buf, len);
#else
    Q_UNUSED(ifname);
    return wifi_wait_for_event(buf, len);
#endif
}

static int q_wifi_command(const char *ifname, const char *command, char *reply, size_t *reply_len)
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
#endif

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
        if (QT_WIFI_DEBUG) qDebug("WiFi event thread is running");
        QWifiManagerEvent *event = 0;
        char buffer[2048];
        while (1) {
            int size = q_wifi_wait_for_event(m_if.constData(), buffer, sizeof(buffer) - 1);
            if (size > 0) {
                buffer[size] = 0;
                event = 0;
                if (strstr(buffer, "SCAN-RESULTS")) {
                    if (m_manager->exitingEventThread()) {
                        if (QT_WIFI_DEBUG) qDebug() << "Exiting WiFi event thread";
                        return;
                    }
                    event = new QWifiManagerEvent(WIFI_SCAN_RESULTS);
                } else if (strstr(buffer, "CONNECTED")) {
                    event = new QWifiManagerEvent(WIFI_CONNECTED);
                } else if (strstr(buffer, "Handshake failed")) {
                    event = new QWifiManagerEvent(WIFI_HANDSHAKE_FAILED);
                } else if (strstr(buffer, "TERMINATING")) {
                    // stop monitoring for events when supplicant is terminating
                    return;
                }
                if (event)
                    QCoreApplication::postEvent(m_manager, event);
            }
        }
    }

    QWifiManager *m_manager;
    QByteArray m_if;
};

/*!
    \qmlmodule Qt.labs.wifi 0.1
    \title WiFi Module
    \ingroup qtee-qmlmodules
    \brief Controlling wireless network interfaces.

    Provides QML types for controlling and accessing information about wireless network interfaces.

    The import command for adding these QML types is:

    \code
    import Qt.labs.wifi 0.1
    \endcode

    If the module is imported into a namespace, some additional methods become available through the
    \l Interface element.

    \code
    import Qt.labs.wifi 0.1 as Wifi
    \endcode

    \section1 QML Types
*/

/*!

    \qmltype WifiManager
    \inqmlmodule Qt.labs.wifi
    \ingroup wifi-qmltypes
    \brief Provides information about the WiFi backend and available networks.

    This element is the main interface to the WiFi functionality.

 */

/*!
    \qmlproperty enumeration WifiManager::networkState

    This property holds the current state of the network connection.

    \list
    \li \e WifiManager.Disconnected - Not connected to any network
    \li \e WifiManager.Authenticating - Verifying password with the network provider
    \li \e WifiManager.HandshakeFailed - Incorrect password provided
    \li \e WifiManager.ObtainingIPAddress - Requesting IP address from DHCP server
    \li \e WifiManager.DhcpRequestFailed - Could not retrieve IP address
    \li \e WifiManager.Connected - Ready to process network requests
    \endlist
*/

/*!
    \qmlproperty bool WifiManager::backendReady

    This property holds whether or not the backend has been successfully initialized.

    \code
    WifiManager {
        id: wifiManager
        scanning: backendReady
    }

    Button {
        id: wifiOnOffButton
        text: (wifiManager.backendReady) ? "Switch Off" : "Switch On"
        onClicked: {
            if (wifiManager.backendReady) {
                wifiManager.stop()
            } else {
                wifiManager.start()
            }
        }
    }
    \endcode
*/

/*!
    \qmlproperty bool WifiManager::scanning

    This property holds whether or not the backend is scanning for WiFi networks. To
    preserve battery energy, stop scanning for networks once you are done with configuring a network.

    Before starting to scan for networks, you need to initialize the WiFi backend.

    \sa start
*/

/*!
    \qmlproperty string WifiManager::connectedSSID

    This property holds the network name.
*/

/*!
    \qmlproperty WifiNetworkListModel WifiManager::networks

    This property holds a list of networks that can be sensed by a device. Use the returned
    model as data model in ListView, model is updated every 5 seconds.

    WifiNetworkListModel is a simple data model consisting of WifiNetwork objects, accessed with
    the "network" data role. Instances of WifiNetwork cannot be created directly from the QML system.

    \code
    WifiManager {
        id: wifiManager
        scanning: backendReady
        Component.onCompleted: start()
    }

    Component {
        id: listDelegate
        Rectangle {
            id: delegateBackground
            height: 60
            width: parent.width
            color: "#5C5C5C"
            border.color: "black"
            border.width: 1

            Text {
                id: ssidLabel
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 10
                font.pixelSize: 20
                font.bold: true
                color: "#E6E6E6"
                text: network.ssid
            }

            Rectangle {
                width: Math.max(100 + network.signalStrength, 0) / 100 * parent.width;
                height: 20
                radius: 10
                antialiasing: true
                anchors.margins: 20
                anchors.right: parent.right
                anchors.top: parent.top
                color: "#BF8888"
                border.color: "#212126"
            }
        }
    }


    ListView {
        id: networkView
        anchors.fill: parent
        model: wifiManager.networks
        delegate: listDelegate
    }
    \endcode

*/

/*!
    \qmlmethod void WifiManager::start()

    Start an initialization of the WiFi backend.

    \sa stop
 */

/*!
    \qmlmethod void WifiManager::stop()

    Stop the WiFi backend and shut down all network functionality.

    \sa start
 */

/*!
    \qmlmethod void WifiManager::connect(WifiNetwork network, const string passphrase)

    Connect to network \a network and use passphrase \a passphrase for authentication.

    \sa disconnect, networkState
 */

/*!
    \qmlmethod void WifiManager::disconnect()

    Disconnect from currently connected network connection.

    \sa connect, networkState
 */

/*!
    \qmlsignal void WifiManager::scanningChanged(bool scanning)

    This signal is emitted when device starts or stops to scan for available wifi networks.

    \sa scanning

*/

/*!
    \qmlsignal void WifiManager::networkStateChanged(WifiNetwork network)

    This signal is emitted whenever changes in a network state occur. Network \a network is the
    the currently active network connection.

    \sa networkState
*/

/*!
    \qmlsignal void WifiManager::backendReadyChanged()

    This signal is emitted when backend has been successfully initialized or shut down.

    \sa start, stop
*/

/*!
    \qmlsignal void WifiManager::connectedSSIDChanged(string ssid)

    This signal is emitted when the device has connected to or disconnected from a network.
    \a ssid contains the name of the connected network, or an empty string if the network was disconnected.

    \sa connect, disconnect
*/

QWifiManager::QWifiManager()
    : m_networkListModel(this)
    , m_eventThread(0)
    , m_scanTimer(0)
    , m_scanning(false)
#ifdef Q_OS_ANDROID
    , m_daemonClientSocket(0)
#endif
    , m_exitingEventThread(false)
    , m_startingUp(true)
    , m_network(0)
{
#ifdef Q_OS_ANDROID
    char interface[PROPERTY_VALUE_MAX];
    property_get(WIFI_INTERFACE, interface, NULL);
    m_interface = interface;
#else
    m_interface = "wlan0"; // use envvar for the interface name?
    m_dhcpRunner = new ProcessRunner(m_interface);
    QObject::connect(m_dhcpRunner, &ProcessRunner::processFinished, this, &QWifiManager::handleDhcpFinished);
#endif
    qDebug("QWifiManager: using wifi interface: %s", m_interface.constData());
    m_eventThread = new QWifiManagerEventThread(this, m_interface);
#ifdef Q_OS_ANDROID
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
    char backendStatus[PROPERTY_VALUE_MAX];
    if (property_get(QT_WIFI_BACKEND, backendStatus, NULL)) {
        if (strcmp(backendStatus, "running") == 0) {
            // let it re-connect, in most cases this will see that everything is working properly
            // and will do nothing. Special case is when process has crashed or was killed by a system
            // signal in previous execution, which results in broken connection to a supplicant,
            // connectToBackend will fix it..
            connectToBackend();
        } else if (strcmp(backendStatus, "stopped") == 0) {
            // same here, cleans up the state
            disconnectFromBackend();
        }
    } else {
#endif
        m_backendReady = false;
        emit backendReadyChanged();
#ifdef Q_OS_ANDROID
    }
#endif
}

QWifiManager::~QWifiManager()
{
    exitEventThread();
    delete m_eventThread;
#ifdef Q_OS_ANDROID
    delete m_daemonClientSocket;
#endif
}

#ifdef Q_OS_ANDROID
void QWifiManager::handleDhcpReply()
{
    if (m_daemonClientSocket->canReadLine()) {
        QByteArray receivedMessage;
        receivedMessage = m_daemonClientSocket->readLine(m_daemonClientSocket->bytesAvailable());
        if (QT_WIFI_DEBUG) qDebug() << "QWifiManager: reply from qconnectivity: " << receivedMessage;
        if (receivedMessage == "success") {
            updateNetworkState(Connected);
            emit connectedSSIDChanged(m_connectedSSID);
            // Store settings of a working wifi connection
            call("SAVE_CONFIG");
        } else if (receivedMessage == "failed") {
            updateNetworkState(DhcpRequestFailed);
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
#endif

void QWifiManager::handleDhcpFinished()
{
    // ### TODO - could be that dhcp request fails, how to determine?
    updateNetworkState(Connected);
    call("SAVE_CONFIG");
}

void QWifiManager::start()
{
    if (QT_WIFI_DEBUG) qDebug("QWifiManager: connecting to the backend");
    if (m_backendReady)
        return;
    connectToBackend();
}

void QWifiManager::stop()
{
    if (QT_WIFI_DEBUG) qDebug("QWifiManager: shutting down");
    if (!m_backendReady)
        return;
    disconnectFromBackend();
}

void QWifiManager::connectToBackend()
{
    // ### TODO: maybe it makes sense to move this functions in non-gui thread
#ifdef Q_OS_ANDROID
    if (!(is_wifi_driver_loaded() || wifi_load_driver() == 0)) {
        qWarning("QWifiManager: failed to load a driver");
        return;
    }
#else
    QProcess::execute(QStringLiteral("ifup"), QStringList() << m_interface.constData());
#endif
    if (q_wifi_start_supplicant() != 0) {
        qWarning("QWifiManager: failed to start a supplicant");
        return;
    }
#ifdef Q_OS_ANDROID
    if (wait_for_property(SUPPLICANT_SVC, "running", 5) < 0) {
        qWarning("QWifiManager: Timed out waiting for supplicant to start");
        return;
    }
#endif
    if (q_wifi_connect_to_supplicant(m_interface.constData()) == 0) {
        m_backendReady = true;
        emit backendReadyChanged();
#ifdef Q_OS_ANDROID
        property_set(QT_WIFI_BACKEND, "running");
#endif
    } else {
        qWarning("QWifiManager: failed to connect to a supplicant");
        return;
    }
    if (QT_WIFI_DEBUG) qDebug("QWifiManager: started successfully");
    m_exitingEventThread = false;
    m_eventThread->start();
    call("SCAN");
}

void QWifiManager::disconnectFromBackend()
{
    exitEventThread();
    if (q_wifi_stop_supplicant() < 0)
        qWarning("QWifiManager: failed to stop supplicant");
    q_wifi_close_supplicant_connection(m_interface.constData());
    setScanning(false);
#ifdef Q_OS_ANDROID
    property_set(QT_WIFI_BACKEND, "stopped");
#else
    QProcess::execute(QStringLiteral("ifdown"), QStringList() << m_interface.constData());
#endif
    m_backendReady = false;
    emit backendReadyChanged();
}

void QWifiManager::exitEventThread()
{
    if (m_eventThread->isRunning()) {
        m_exitingEventThread = true;
        call("SCAN");
        m_eventThread->wait();
    }
}

void QWifiManager::setScanning(bool scanning)
{
    if (!m_backendReady || m_scanning == scanning)
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
    QByteArray cmd;
#ifdef Q_OS_ANDROID
#if !(Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR < 4)
    cmd.append("IFNAME=").append(m_interface).append(" ");
#endif
#endif
    cmd.append(command);
    if (q_wifi_command(m_interface.constData(), cmd.constData(), data, &len) < 0) {
        qWarning("QWifiManager: call failed: %s", cmd.constData());
        return QByteArray();
    }
    if (len < sizeof(data))
        data[len] = 0;
    QByteArray result = QByteArray::fromRawData(data, len);
    if (QT_WIFI_DEBUG) qDebug("QWifiManager::call: %s ==>\n%s", cmd.constData(), result.constData());
    return result;
}

bool QWifiManager::checkedCall(const char *command) const
{
    return call(command).trimmed().toUpper() == "OK";
}

void QWifiManager::updateNetworkState(NetworkState state)
{
    m_state = state;
    if (m_network)
        emit networkStateChanged(m_network);
}

bool QWifiManager::event(QEvent *e)
{
    switch ((int) e->type()) {
    case WIFI_SCAN_RESULTS:
        m_networkListModel.parseScanResults(call("SCAN_RESULTS"));
        if (m_startingUp)
            handleConnected();
        return true;
    case WIFI_CONNECTED:
        handleConnected();
        break;
    case WIFI_HANDSHAKE_FAILED:
        updateNetworkState(HandshakeFailed);
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
    m_network = network;
    if (network->ssid() == m_connectedSSID) {
        if (QT_WIFI_DEBUG)
            qDebug("QWifiManager::connect(), already connected to %s", network->ssid().constData());
        return;
    }
    updateNetworkState(Authenticating);
    call("DISABLE_NETWORK all");
    if (!m_connectedSSID.isEmpty()) {
        m_connectedSSID.clear();
        emit connectedSSIDChanged(m_connectedSSID);
    }

    bool networkKnown = false;
    QByteArray id;
    QByteArray listResult = call("LIST_NETWORKS");
    QList<QByteArray> lines = listResult.split('\n');
    foreach (const QByteArray &line, lines) {
        if (line.contains(network->ssid())) {
            id = line.split('\t').at(0);
            networkKnown = true;
            break;
        }
    }

    if (!networkKnown) {
        bool ok;
        id = call("ADD_NETWORK").trimmed();
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
#ifdef Q_OS_ANDROID
    QByteArray req = m_interface;
    sendDhcpRequest(req.append(" disconnect"));
#endif
    m_connectedSSID.clear();
    updateNetworkState(Disconnected);
    emit connectedSSIDChanged(m_connectedSSID);
}

void ProcessRunner::run()
{
    // kill existing udhcpc instance
    QString filePath = QString("/var/run/udhcpc.").append(m_ifc).append(".pid");
    QFile pidFile(filePath);
    if (pidFile.open(QIODevice::ReadOnly)) {
        QByteArray pid = pidFile.readAll();
        pidFile.close();
        QProcess::execute(QStringLiteral("kill"), QStringList() << pid.trimmed().constData());
    } else {
        qWarning() << "QWifiManager - Failed to read" << filePath;
    }
    QStringList args;
    args << QStringLiteral("-R") << QStringLiteral("-n") << QStringLiteral("-p")
         << filePath << QStringLiteral("-i") << m_ifc;
    // start DHCP client
    QProcess::execute(QStringLiteral("udhcpc"), args);
    emit processFinished();
}

void QWifiManager::handleConnected()
{
    QList<QByteArray> lists = call("LIST_NETWORKS").split('\n');
    QByteArray connectedNetwork;
    for (int i = 1; i < lists.size(); ++i) {
        if (lists.at(i).toUpper().contains("[CURRENT]")) {
            connectedNetwork = lists.at(i);
            break;
        }
    }

    if (connectedNetwork.isEmpty()) {
        if (QT_WIFI_DEBUG) qDebug("QWifiManager::handleConnected: not connected to a network...");
        return;
    }

    if (QT_WIFI_DEBUG) qDebug("QWifiManager::handleConnected: current is %s", connectedNetwork.constData());

    QList<QByteArray> info = connectedNetwork.split('\t');
    m_connectedSSID = info.at(1);

    if (m_startingUp) {
        m_startingUp = false;
        if (!m_network) {
            int pos = 0; // unused
            m_network = m_networkListModel.networkForSSID(info.at(1), &pos);
        }
    }

    updateNetworkState(ObtainingIPAddress);
#ifdef Q_OS_ANDROID
    QByteArray req = m_interface;
    sendDhcpRequest(req.append(" connect"));
#else
    m_dhcpRunner->start();
#endif
}
