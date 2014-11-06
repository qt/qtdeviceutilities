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
#include "qwifinetworklistmodel_p.h"
#include "qwifinetwork_p.h"
#include "qwifimanager_p.h"
#include "qwifidevice.h"

#include <QtCore/QFile>
#include <QtCore/QProcess>

QT_BEGIN_NAMESPACE

/*!
    \qmlmodule QtWifi 1.0
    \title QtWifi Module
    \ingroup qtee-qmlmodules
    \brief A module for managing wireless network connectivity.

    Provides QML types for controlling WiFi networks - handling WiFi backend initialization,
    retrieving information from nearby WiFi access points, setting up and bringing down WiFi
    connections, querying DHCP server for IP address.

    The import command for adding these QML types is:

    \code
    import QtWifi 1.0
    \endcode

    \section1 API Reference
*/

/*!

    \qmltype WifiManager
    \inqmlmodule QtWifi
    \ingroup wifi-qmltypes
    \brief Main interface to the WiFi functionality.

    WifiManager is a singleton type that provides information about the WiFi backend and
    available networks, use it to control the WiFi backend, scan for wireless networks
    and connect to selected network. WifiManager provides events for backend and network
    state changes.

 */

/*!
    \qmlsignal void WifiManager::networkStateChanged(NetworkState networkState)

    This signal is emitted whenever changes in a network state occur. The network name for
    which the state changes events are send can be obtained from currentSSID.

    \sa networkState
*/

/*!
    \qmlsignal void WifiManager::backendStateChanged(BackendState backendState)

    This signal is emitted whenever changes in a backend state occur.

    \sa start, stop
*/

/*!
    \qmlsignal void WifiManager::currentSSIDChanged(string currentSSID)

    This signal is emitted when switching between different WiFi networks.

    \sa start, stop
*/

/*!
    \qmlsignal void WifiManager::scanningChanged(bool scanning)

    This signal is emitted when device starts or stops to scan for available wifi networks.

    \sa scanning

*/

// must be in the same order as in enum {} definiton
const char *nsText[] = { "Disconnected", "Authenticating", "HandshakeFailed",
                         "ObtainingIPAddress", "DhcpRequestFailed", "Connected" };
const char *bsText[] = { "Initializing", "Running", "Terminating", "NotRunning" };

QWifiManagerPrivate::QWifiManagerPrivate(QWifiManager *manager)
        : q_ptr(manager)
        , m_networkListModel(new QWifiNetworkListModel())
        , m_scanTimer(0)
        , m_scanning(false)
        , m_interface(QWifiDevice::wifiInterfaceName())
        , m_backendState(QWifiManager::NotRunning)
        , m_networkState(QWifiManager::Disconnected)
        , m_setCurrentSSID(true)
{
    qCDebug(B2QT_WIFI) << "using wifi interface: " << m_interface;
}

QWifiManagerPrivate::~QWifiManagerPrivate()
{
    delete m_wifiController;
    delete m_networkListModel;
}

QString QWifiManagerPrivate::getConnectedNetwork()
{
    QStringList lists = call(QStringLiteral("LIST_NETWORKS")).split('\n');
    QString connectedNetwork;
    for (int i = 1; i < lists.size(); ++i) {
        if (lists.at(i).toUpper().contains(QStringLiteral("[CURRENT]"))) {
            connectedNetwork = lists.at(i);
            break;
        }
    }
    return connectedNetwork;
}

void QWifiManagerPrivate::emitCurrentSSIDChanged()
{
    Q_Q(QWifiManager);
    if (m_previousSSID != m_currentSSID) {
        qCDebug(B2QT_WIFI) << "current SSID: " << m_previousSSID << " -> " << m_currentSSID;
        m_previousSSID = m_currentSSID;
        emit q->currentSSIDChanged(m_currentSSID);
    }
}

void QWifiManagerPrivate::setCurrentSSID()
{
    qCDebug(B2QT_WIFI, "setCurrentSSID");
    m_setCurrentSSID = false;
    QString connectedNetwork = getConnectedNetwork();
    if (!connectedNetwork.isEmpty()) {
        QString ssid = connectedNetwork.split('\t').at(1);
        QWifiNetwork *network = m_networkListModel->networkForSSID(ssid);
        if (network) {
            m_currentSSID = network->ssid();
            emitCurrentSSIDChanged();
            if (call(QStringLiteral("STATUS")).contains(QStringLiteral("wpa_state=COMPLETED")))
                updateNetworkState(QWifiManager::Connected);
        }
    }
}

void QWifiManagerPrivate::handleConnected()
{
    qCDebug(B2QT_WIFI, "handleConnected");
    QString connectedNetwork = getConnectedNetwork();
    if (connectedNetwork.isEmpty())
        return;

    m_currentSSID = connectedNetwork.split('\t').at(1);
    qCDebug(B2QT_WIFI) << "connected network: " << m_currentSSID;
    updateNetworkState(QWifiManager::ObtainingIPAddress);
    m_wifiController->call(QWifiController::AcquireIPAddress);
}

void QWifiManagerPrivate::handleDisconneced()
{
    updateNetworkState(QWifiManager::Disconnected);
}

void QWifiManagerPrivate::updateNetworkState(QWifiManager::NetworkState networkState)
{
    Q_Q(QWifiManager);
    qCDebug(B2QT_WIFI, "network state: %s -> %s", nsText[m_networkState], nsText[networkState]);
    if (m_networkState == networkState)
        return;

    m_networkState = networkState;
    emit q->networkStateChanged(m_networkState);
}

void QWifiManagerPrivate::updateBackendState(QWifiManager::BackendState backendState)
{
    Q_Q(QWifiManager);
    qCDebug(B2QT_WIFI, "backend state: %s -> %s", bsText[m_backendState], bsText[backendState]);
    if (m_backendState == backendState)
        return;

    m_backendState = backendState;
    emit q->backendStateChanged(m_backendState);
}

void QWifiManagerPrivate::updateWifiState()
{
    bool supplicantRunning = false;
#ifdef Q_OS_ANDROID_NO_SDK
    char supplicantState[PROPERTY_VALUE_MAX];
    if (property_get("init.svc.wpa_supplicant", supplicantState, 0)) {
        if (strcmp(supplicantState, "running") == 0)
            supplicantRunning = true;
    }
#else
    QProcess ps;
    ps.start(QStringLiteral("ps"));
    ps.waitForFinished();
    if (ps.readAll().contains("wpa_supplicant"))
        supplicantRunning = true;
#endif
    if (supplicantRunning) {
        m_wifiController->resetSupplicantSocket();
        m_wifiController->startWifiEventThread();
        m_backendState = QWifiManager::Running;
    }
}

QString QWifiManagerPrivate::call(const QString &command)
{
    if (m_backendState != QWifiManager::Running)
        return QString();

    char data[2048];
    size_t len = sizeof(data) - 1; // -1: room to add a 0-terminator
    QString actualCommand = command;
#ifdef Q_OS_ANDROID_NO_SDK
#if !(Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR < 4)
    QString prefix = QLatin1String("IFNAME=" + m_interface + " ");
    actualCommand.prepend(prefix);
#endif
#endif
    if (q_wifi_command(m_interface, actualCommand.toLatin1(), data, &len) < 0) {
        qCDebug(B2QT_WIFI) << "call to supplicant failed: " << actualCommand;
        return QString();
    }
    if (len < sizeof(data))
        data[len] = 0;

    QString result = QLatin1String(data);
    return result;
}

bool QWifiManagerPrivate::checkedCall(const QString &command)
{
    return call(command).trimmed().toUpper() == QLatin1String("OK");
}

void QWifiManagerPrivate::updateLastError(const QString &error)
{
    Q_Q(QWifiManager);
    qCWarning(B2QT_WIFI) << error;
    m_lastError = error;
    emit q->lastErrorChanged(m_lastError);
}


QWifiManager* QWifiManager::m_instance = 0;
QWifiManager* QWifiManager::instance()
{
    if (!m_instance)
        m_instance = new QWifiManager();
    return m_instance;
}

QWifiManager::QWifiManager()
    : d_ptr(new QWifiManagerPrivate(this))
{
    Q_D(QWifiManager);

    if (!QWifiDevice::wifiSupported())
        qCWarning(B2QT_WIFI) << "WifiManager may not work as expected on this device. Use the API provided by QtWifi "
                      "library to verify if device has support for Wi-Fi before creating an instance of wifi manager!";

    d->m_wifiController = new QWifiController(this, d_ptr);
    QObject::connect(d->m_wifiController, &QWifiController::backendStateChanged,
                     this, &QWifiManager::handleBackendStateChanged);
    QObject::connect(d->m_wifiController, &QWifiController::dhcpRequestFinished,
                     this, &QWifiManager::handleDhcpRequestFinished);
    d->m_wifiController->start();

    d->updateWifiState();
}

QWifiManager::~QWifiManager()
{
    Q_D(QWifiManager);
    d->m_wifiController->call(QWifiController::ExitEventLoop);
    d->m_wifiController->wait();
    delete d_ptr;
}

/*!
    \qmlproperty WifiNetworkListModel WifiManager::networks
    \readonly

    This property holds a list of networks that can be sensed by a device. Use the returned
    model as data model in ListView, model is updated every 5 seconds.

    WifiNetworkListModel is a simple data model consisting of WifiNetwork objects, accessed with
    the "network" data role name. Instances of WifiNetwork cannot be created directly from the QML system.

*/

QAbstractListModel *QWifiManager::networks() const
{
    Q_D(const QWifiManager);
    return d->m_networkListModel;
}

/*!
    \qmlproperty string WifiManager::currentSSID
    \readonly

    This property holds the network name for which the networkState changes events are sent or
    or an empty string when there is no active network.
*/

QString QWifiManager::currentSSID() const
{
    Q_D(const QWifiManager);
    return d->m_currentSSID;
}

/*!
    \qmlproperty enumeration WifiManager::networkState
    \readonly

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

QWifiManager::NetworkState QWifiManager::networkState() const
{
    Q_D(const QWifiManager);
    return d->m_networkState;
}

/*!
    \qmlproperty enumeration WifiManager::backendState
    \readonly

    This property holds the current state of the WiFi backend.

    \list
    \li \e WifiManager.Initializing - Wireless supplicant is starting up
    \li \e WifiManager.Running - Supplicant is initialized and ready to process commands
    \li \e WifiManager.Terminating - Shutting down wireless supplicant
    \li \e WifiManager.NotRunning - Wireless supplicant process is not running
    \endlist
*/

QWifiManager::BackendState QWifiManager::backendState() const
{
    Q_D(const QWifiManager);
    return d->m_backendState;
}

/*!
    \qmlmethod void WifiManager::start()

    Start the WiFi backend. This function returns immediately, the backendState
    change events are delivered asynchronously.

    \sa stop, backendState
 */

void QWifiManager::start()
{
    Q_D(QWifiManager);
    d->m_wifiController->call(QWifiController::InitializeBackend);
}

/*!
    \qmlmethod void WifiManager::stop()

    Stop the WiFi backend and if connected to any network shut down network connection.
    This function returns immediately, the backendState change events are delivered asynchronously.

    \sa start, backendState
 */

void QWifiManager::stop()
{
    Q_D(QWifiManager);
    d->m_wifiController->call(QWifiController::TerminateBackend);
}

void QWifiManager::handleBackendStateChanged(BackendState backendState)
{
    Q_D(QWifiManager);
    switch (backendState) {
    case Running:
        d->m_setCurrentSSID = true;
        break;
    case NotRunning:
        d->updateNetworkState(Disconnected);
        break;
    default:
        break;
    }
    d->updateBackendState(backendState);
}

void QWifiManager::handleDhcpRequestFinished(const QString &status)
{
    Q_D(QWifiManager);
    qCDebug(B2QT_WIFI) << "handleDhcpRequestFinished: " << status << " for " << d->m_currentSSID;
    if (status == QLatin1String("success")) {
        d->emitCurrentSSIDChanged();
        d->updateNetworkState(Connected);
        d->call(QStringLiteral("SAVE_CONFIG"));
    } else {
        d->updateNetworkState(DhcpRequestFailed);
    }
}

void QWifiManager::setScanning(bool scanning)
{
    Q_D(QWifiManager);
    if (d->m_scanning == scanning)
        return;

    d->m_scanning = scanning;
    emit scanningChanged(d->m_scanning);
    if (d->m_scanning) {
        d->call(QStringLiteral("SCAN"));
        // ### TODO android has property for this - wifi.supplicant_scan_interval
        d->m_scanTimer = startTimer(5000);
    } else {
        killTimer(d->m_scanTimer);
    }
}

/*!
    \qmlproperty bool WifiManager::scanning

    This property holds whether or not the backend is scanning for WiFi networks. To
    preserve battery energy, set this property to false when scanning is not required.

    Before starting to scan for networks, you need to initialize the WiFi backend.

    \sa start
*/

bool QWifiManager::scanning() const
{
    Q_D(const QWifiManager);
    return d->m_scanning;
}

QString QWifiManager::lastError() const
{
    Q_D(const QWifiManager);
    return d->m_lastError;
}

bool QWifiManager::event(QEvent *event)
{
    Q_D(QWifiManager);
    switch ((int) event->type()) {
    case WIFI_SCAN_RESULTS:
        d->m_networkListModel->parseScanResults(d->call(QStringLiteral("SCAN_RESULTS")));
        if (d->m_setCurrentSSID || d->m_currentSSID.isEmpty())
            d->setCurrentSSID();
        return true;
    case WIFI_CONNECTED:
        d->handleConnected();
        return true;
    case WIFI_DISCONNECTED:
        d->handleDisconneced();
        return true;
    case WIFI_AUTHENTICATING:
        d->updateNetworkState(Authenticating);
        d->emitCurrentSSIDChanged();
        return true;
    case WIFI_HANDSHAKE_FAILED:
        d->updateNetworkState(HandshakeFailed);
        return true;
    case QEvent::Timer: {
        int tid = static_cast<QTimerEvent *>(event)->timerId();
        if (tid == d->m_scanTimer) {
            d->call(QStringLiteral("SCAN"));
            return true;
        }
        break;
    }
    }
    return QObject::event(event);
}


/*!
    \qmlmethod void WifiManager::connect(WifiNetwork network, string passphrase)

    Connect to network \a network and use passphrase \a passphrase for authentication.

    \sa disconnect, networkState
 */

bool QWifiManager::connect(QWifiConfiguration *config)
{
    Q_D(QWifiManager);
    if (d->m_backendState != Running) {
        qCWarning(B2QT_WIFI) << "start wifi backend before calling connect() !";
        return false;
    }

    d->call(QStringLiteral("DISABLE_NETWORK all"));

    d->m_currentSSID = config->ssid();
    bool networkKnown = false;
    QString id;
    QString listResult = d->call(QStringLiteral("LIST_NETWORKS"));
    QStringList lines = listResult.split('\n');
    foreach (const QString &line, lines) {
        if (line.contains(d->m_currentSSID)) {
            id = line.split('\t').at(0);
            networkKnown = true;
            break;
        }
    }

    if (!networkKnown) {
        bool ok;
        id = d->call(QStringLiteral("ADD_NETWORK")).trimmed();
        id.toInt(&ok);
        if (!ok) {
            d->updateLastError(QStringLiteral("failed to add network!"));
            return false;
        }
    }

    bool ok = true;
    QChar q = QLatin1Char('"');
    QString setNetworkCommand = QLatin1String("SET_NETWORK ") + id;
    if (!networkKnown) {
        ok = ok && d->checkedCall(setNetworkCommand + QLatin1String(" ssid ") + q + d->m_currentSSID + q);
    }

    QString key_mgmt;
    QString protocol = config->protocol().toUpper();
    QString psk = config->passphrase();
    // ref: http://w1.fi/cgit/hostap/plain/wpa_supplicant/wpa_supplicant.conf
    if (protocol.isEmpty() || protocol.contains(QStringLiteral("WPA"))) {
        // ### todo - password length has limits (see IEEE 802.11), we need to check
        // for those limits here. Supplicant gives only a meaningless "fail" message.
        ok = ok && d->checkedCall(setNetworkCommand + QLatin1String(" psk ") + q + psk + q);
        key_mgmt = QLatin1String("WPA-PSK");
    } else if (protocol.contains(QStringLiteral("WEP"))) {
        ok = ok && d->checkedCall(setNetworkCommand + QLatin1String(" wep_key0 ") + q + psk + q);
        ok = ok && d->checkedCall(setNetworkCommand + QLatin1String(" auth_alg OPEN SHARED"));
        key_mgmt = QLatin1String("NONE");
    } else if (protocol.contains(QStringLiteral("WPS")) && psk.length() == 0) {
        // open network
        key_mgmt = QLatin1String("NONE");
    }
    ok = ok && d->checkedCall(setNetworkCommand + QLatin1String(" key_mgmt ") + key_mgmt);
    if (!ok) {
        if (!networkKnown)
            d->call(QLatin1String("REMOVE_NETWORK ") + id);
        d->updateLastError(QLatin1String("failed to set properties on network: ") + id);
        return false;
    }

    d->call(QLatin1String("SELECT_NETWORK ") + id);
    d->call(QStringLiteral("RECONNECT"));

    return true;
}

/*!
    \qmlmethod void WifiManager::disconnect()

    Disconnect from currently connected network connection.

    \sa connect, networkState
*/

void QWifiManager::disconnect()
{
    Q_D(QWifiManager);
    d->call(QStringLiteral("DISCONNECT"));
    d->m_wifiController->call(QWifiController::StopDhcp);
}

QT_END_NAMESPACE
