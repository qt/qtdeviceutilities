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
#include "qwifisupplicant_p.h"

#include "qwifidevice.h"

#include <QtCore/QFile>
#include <QtCore/QProcess>

QT_BEGIN_NAMESPACE

// must be in the same order as in enum {} definiton
const char *nsText[] = { "Disconnected", "Authenticating", "HandshakeFailed",
                         "ObtainingIPAddress", "DhcpRequestFailed", "Connected" };
const char *bsText[] = { "Initializing", "Running", "Terminating", "NotRunning" };

QWifiManagerPrivate::QWifiManagerPrivate(QWifiManager *manager)
        : q_ptr(manager)
        , m_networkListModel(new QWifiNetworkListModel(manager))
        , m_scanTimer(0)
        , m_scanning(false)
        , m_interface(QWifiDevice::wifiInterfaceName())
        , m_backendState(QWifiManager::NotRunning)
        , m_networkState(QWifiManager::Disconnected)
{
    qCDebug(B2QT_WIFI) << "using wifi interface: " << m_interface;
}

QWifiManagerPrivate::~QWifiManagerPrivate()
{
    delete m_wifiController;
}

void QWifiManagerPrivate::setCurrentSSID(const QString &ssid)
{
    Q_Q(QWifiManager);
    qCDebug(B2QT_WIFI) << "current SSID: " << m_currentSSID << " -> " << ssid;
    if (m_currentSSID == ssid)
        return;

    m_currentSSID = ssid;
    emit q->currentSSIDChanged(m_currentSSID);
}

void QWifiManagerPrivate::handleAuthenticating(QWifiEvent *event)
{
    QString data = event->data().trimmed();
    QString ssid = data.mid(data.indexOf(QLatin1String("SSID")) + 6);
    ssid = ssid.left(ssid.lastIndexOf(QLatin1Char('\'')));

    setCurrentSSID(QWifiSupplicant::decodeSsid(ssid));
    updateNetworkState(QWifiManager::Authenticating);
}

void QWifiManagerPrivate::handleConnected()
{
    qCDebug(B2QT_WIFI) << "connected network: " << m_currentSSID;
    updateNetworkState(QWifiManager::ObtainingIPAddress);
    m_wifiController->asyncCall(QWifiController::AcquireIPAddress);
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
    QProcess ps;
    ps.start(QStringLiteral("ps"));
    ps.waitForFinished();
    bool supplicantRunning = ps.readAll().contains("wpa_supplicant");
    if (supplicantRunning && m_wifiController->resetSupplicantSocket())
        m_backendState = QWifiManager::Running;
}

QString QWifiManagerPrivate::call(const QString &command)
{
    if (m_backendState != QWifiManager::Running)
        return QString();

    QByteArray reply;
    bool success = m_wifiController->supplicant()->sendCommand(command, &reply);
    if (!success) {
        qCDebug(B2QT_WIFI) << "call to supplicant failed!";
        return QString();
    }

    return QLatin1String(reply.trimmed());
}

bool QWifiManagerPrivate::checkedCall(const QString &command)
{
    return call(command).toUpper() == QLatin1String("OK");
}

void QWifiManagerPrivate::updateLastError(const QString &error)
{
    Q_Q(QWifiManager);
    qCWarning(B2QT_WIFI) << error;
    m_lastError = error;
    emit q->lastErrorChanged(m_lastError);
}

/*!
    \class QWifiManager
    \inmodule B2Qt.Wifi.Cpp
    \ingroup wifi-cppclasses
    \brief Enables an application to be Wifi-capable.

    QWifiManager is a singleton class that handles Wifi-related tasks. You can
    use QWifiManager to control the Wifi backend, scan for Wifi access points,
    and connect to a wireless network.

    QWifiManager packs the scan results in a list-based data model, which can
    be used with Qt's Model/View classes. Information about a Wifi network can
    be accessed using the QWifiManager::Roles data roles.
*/

/*!
    \enum QWifiManager::NetworkState

    Describes current state of the network connection.

    \value Disconnected Not connected to any network
    \value Authenticating Verifying password with the network provider
    \value HandshakeFailed Incorrect password provided
    \value ObtainingIPAddress Requesting IP address from DHCP server
    \value DhcpRequestFailed Could not retrieve IP address
    \value Connected Ready to process network requests
*/

/*!
    \enum QWifiManager::BackendState

    Describes current state of the Wifi backend.

    \value Initializing Wireless supplicant is starting up
    \value Running Supplicant is initialized and ready to process commands
    \value Terminating Shutting down wireless supplicant
    \value NotRunning Wireless supplicant process is not running
*/

/*!
    \enum QWifiManager::Roles

    Data roles supported by the data model returned from QWifiManager::networks()

    \value SSID informal (human) name of a Wifi network (QString)
    \value BSSID basic service set identification of a network, used to uniquely identify BSS (QString)
    \value SignalStrength strength of a Wifi signal represented as percentage (0-100) (int)
    \value WPASupported holds whether network access point supports WPA security protocol (bool)
    \value WPA2Supported holds whether network access point supports WPA2 security protocol (bool)
    \value WEPSupported holds whether network access point supports WEP security protocol (bool)
    \value WPSSupported holds whether network access point supports WPS security protocol (bool)
*/

QWifiManager* QWifiManager::m_instance = 0;

/*!
    Returns a singleton instance of QWifiManager.
*/
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

/*!
    Destroys the QWifiManager singleton instance.
 */
QWifiManager::~QWifiManager()
{
    Q_D(QWifiManager);
    d->m_wifiController->asyncCall(QWifiController::ExitEventLoop);
    d->m_wifiController->wait();
    delete d_ptr;
}

/*!
    \property QWifiManager::networks
    \brief a list-based data model of networks

    Returns a list-based data model of networks that can be sensed by a device.
    Model can be used with Qt's Model/View classes such as QListView. Data in
    the model is updated every 5 seconds if scanning is enabled.

    \sa isScanning()
*/
QAbstractListModel *QWifiManager::networks() const
{
    Q_D(const QWifiManager);
    return d->m_networkListModel;
}

/*!
    \property QWifiManager::currentSSID
    \brief a network name of the last selected network

    The network for which the NetworkState change signals are emitted.
    Property can contain an empty string when there is no active network
    connection.
*/
QString QWifiManager::currentSSID() const
{
    Q_D(const QWifiManager);
    return d->m_currentSSID;
}

/*!
    \property QWifiManager::networkState
    \brief the current network state

    Returns the current network state.
*/
QWifiManager::NetworkState QWifiManager::networkState() const
{
    Q_D(const QWifiManager);
    return d->m_networkState;
}

/*!
    \property QWifiManager::backendState
    \brief the current backend state.

    Returns the current backend state.
*/
QWifiManager::BackendState QWifiManager::backendState() const
{
    Q_D(const QWifiManager);
    return d->m_backendState;
}

/*!
    Start the Wifi backend. This function returns immediately, the BackendState
    change events are delivered asynchronously.

    \sa stop(), BackendState
*/
void QWifiManager::start()
{
    Q_D(QWifiManager);
    d->m_wifiController->asyncCall(QWifiController::InitializeBackend);
}

/*!
    Stop the Wifi backend. Closes the open network connection if any.
    This function returns immediately, and the BackendState change events are
    delivered asynchronously.

    \sa start(), BackendState
*/
void QWifiManager::stop()
{
    Q_D(QWifiManager);
    d->m_wifiController->asyncCall(QWifiController::TerminateBackend);
}

/*!
    \property QWifiManager::scanning
    \brief whether the backend is scanning for Wifi networks.

    Sets whether to scan the environment for Wifi access points.

    To preserve battery energy, set this property to false when scanning is not required.
    When enabled, new readings are taken every 5 seconds.

    \note You must initialize the Wifi backend to scan for networks.

    \sa start()
*/
bool QWifiManager::isScanning() const
{
    Q_D(const QWifiManager);
    return d->m_scanning;
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
        // ### TODO expose this with a property
        d->m_scanTimer = startTimer(5000);
    } else {
        killTimer(d->m_scanTimer);
    }
}

/*!
    \property QWifiManager::lastError
    \brief a QString containing the last error message set by QWifiManager.

    Returns a QString containing the last error message set by QWifiManager.
    This helps in diagnosing the internal process failures.

    \sa connect()
*/
QString QWifiManager::lastError() const
{
    Q_D(const QWifiManager);
    return d->m_lastError;
}

/*! \reimp */
bool QWifiManager::event(QEvent *event)
{
    Q_D(QWifiManager);
    switch ((int) event->type()) {
    case WIFI_SCAN_RESULTS:
        d->m_networkListModel->parseScanResults(d->call(QStringLiteral("SCAN_RESULTS")));
        return true;
    case WIFI_CONNECTED:
        d->handleConnected();
        return true;
    case WIFI_DISCONNECTED:
        d->handleDisconneced();
        return true;
    case WIFI_AUTHENTICATING:
        d->handleAuthenticating(static_cast<QWifiEvent *>(event));
        return true;
    case WIFI_HANDSHAKE_FAILED:
        d->updateNetworkState(QWifiManager::HandshakeFailed);
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
    Connect a device to a network using the \a config network configuration.
    This method returns \c true if the network with the provided configuration
    could be successfully added by the backend or \c false on failure.
    Use lastError() to obtain the error message on failure.

    \sa disconnect(), NetworkState, lastError()
*/
bool QWifiManager::connect(QWifiConfiguration *config)
{
    Q_D(QWifiManager);
    if (d->m_backendState != Running) {
        qCWarning(B2QT_WIFI) << "start wifi backend before calling connect() !";
        return false;
    }

    d->call(QStringLiteral("DISABLE_NETWORK all"));
    d->setCurrentSSID(config->ssid());

    bool networkKnown = false;
    QString id;
    const QStringList configuredNetworks = d->call(QStringLiteral("LIST_NETWORKS")).split('\n');
    for (int i = 1; i < configuredNetworks.length(); ++i) {
        const QStringList networkFields = configuredNetworks.at(i).split('\t');
        const QString ssid = QWifiSupplicant::decodeSsid(networkFields.at(1));
        if (ssid == d->m_currentSSID) {
            id = networkFields.at(0);
            networkKnown = true;
            break;
        }
    }

    if (!networkKnown) {
        bool ok;
        id = d->call(QStringLiteral("ADD_NETWORK"));
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

    // --------------------- configure network ------------------------------
    // ref: http://w1.fi/cgit/hostap/plain/wpa_supplicant/wpa_supplicant.conf
    // ref: https://www.freebsd.org/cgi/man.cgi?wpa_supplicant.conf
    // ----------------------------------------------------------------------
    if (protocol.isEmpty() || protocol.contains(QStringLiteral("WPA"))) {
        // ### TODO - password length has limits (see IEEE 802.11), we need to check
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

    if (config->isSsidHidden())
        ok = ok && d->checkedCall(setNetworkCommand + QLatin1String(" scan_ssid 1"));

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
    Disconnect from currently connected network connection.

    \sa connect(), networkState()
*/
void QWifiManager::disconnect()
{
    Q_D(QWifiManager);
    d->call(QStringLiteral("DISCONNECT"));
    d->m_wifiController->asyncCall(QWifiController::StopDhcp);
}

void QWifiManager::handleBackendStateChanged(BackendState backendState)
{
    Q_D(QWifiManager);
    switch (backendState) {
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
        d->updateNetworkState(Connected);
        d->call(QStringLiteral("SAVE_CONFIG"));
    } else {
        d->updateNetworkState(DhcpRequestFailed);
    }
}

QT_END_NAMESPACE
