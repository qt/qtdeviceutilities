#include "qwifimanager.h"

#include <QtCore>

#include <hardware_legacy/wifi.h>
#include <cutils/properties.h>


#define WLAN_INTERFACE "wlan0"

static bool WIFI_DEBUG = !qgetenv("WIFI_DEBUG").isEmpty();

const QEvent::Type WIFI_SCAN_RESULTS = (QEvent::Type) (QEvent::User + 1);
const QEvent::Type WIFI_CONNECTED = (QEvent::Type) (QEvent::User + 2);

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
    QWifiManagerEventThread(QWifiManager *manager)
        : m_manager(manager)
    {

    }

    void run() {
        if (WIFI_DEBUG) qDebug("EventReceiver thread is running");
        char buffer[2048];
        while (1) {
            int size = wifi_wait_for_event(WLAN_INTERFACE, buffer, sizeof(buffer) - 1);
            if (size > 0) {
                buffer[size] = 0;

                if (WIFI_DEBUG) qDebug("EVENT: %s", buffer);

                char *event = &buffer[11];
                if (strstr(event, "SCAN-RESULTS")) {
                    QWifiManagerEvent *e = new QWifiManagerEvent(WIFI_SCAN_RESULTS);
                    QCoreApplication::postEvent(m_manager, e);
                } else if (strstr(event, "CONNECTED")) {
                    QWifiManagerEvent *e = new QWifiManagerEvent(WIFI_CONNECTED);
                    QCoreApplication::postEvent(m_manager, e);
                }
            }
        }
    }

    QWifiManager *m_manager;
};



QWifiManager::QWifiManager()
    : m_networks(this)
    , m_eventThread(0)
    , m_scanTimer(0)
    , m_internalState(IS_Uninitialized)
    , m_scanning(false)
{
}



void QWifiManager::start()
{
    if (WIFI_DEBUG) qDebug("QWifiManager: start");
    connectToBackend();
}



void QWifiManager::setScanning(bool scanning)
{
    if (m_scanning == scanning)
        return;

    m_scanning = scanning;
    emit scanningChanged(scanning);

    if (m_scanning) {
        if (WIFI_DEBUG) qDebug("QWifiManager: scanning");
        call("SCAN");
        m_scanTimer = startTimer(5000);
    } else {
        if (WIFI_DEBUG) qDebug("QWifiManager: stop scanning");
        killTimer(m_scanTimer);
    }
}



QByteArray int_to_ip(int i) {
    QByteArray ip;

    ip.append(QByteArray::number(i & 0x000000ff));
    ip.append('.');
    ip.append(QByteArray::number((i & 0x0000ff00) >> 8));
    ip.append('.');
    ip.append(QByteArray::number((i & 0x00ff0000) >> 16));
    ip.append('.');
    ip.append(QByteArray::number(i >> 24));

    return ip;
}


void QWifiManager::connectToBackend()
{
    if (m_internalState == IS_Uninitialized)
        m_internalState = IS_StartBackend;

    if (m_internalState == IS_StartBackend
            && wifi_start_supplicant(0) >= 0)
        m_internalState = IS_ConnectToBackend;

    if (m_internalState == IS_ConnectToBackend
            && wifi_connect_to_supplicant(WLAN_INTERFACE) >= 0)
        m_internalState = IS_UpAndRunning;

    if (m_internalState == IS_UpAndRunning) {
        emit readyChanged(true);
        m_eventThread = new QWifiManagerEventThread(this);
        m_eventThread->start();

        qDebug("QWifiManager: started successfully");

        handleConnected();
    } else {
        if (WIFI_DEBUG) qWarning("QWifiManager: stuck at internal state level: %d", m_internalState);
    }
}


QByteArray QWifiManager::call(const char *command)
{
    char data[2048];
    size_t len = sizeof(data);
    if (wifi_command(WLAN_INTERFACE, command, data, &len) < 0) {
        qWarning("QWifiManager: call failed: %s", command);
        return QByteArray();
    }
    QByteArray result = QByteArray::fromRawData(data, len);
    qDebug("QWifiManager::call: %s ==>\n%s", command, result.constData());
    return result;
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
        break; }
    }

    return QObject::event(e);
}

void QWifiManager::connect(QWifiNetwork *network, const QString &passphrase)
{
    if (network->ssid() == m_connectedSSID) {
        if (WIFI_DEBUG) qDebug("QWifiManager::connect(), already connected to %s", network->ssid().constData());
        return;
    }

    QByteArray listResult = call("LIST_NETWORKS");

    if (listResult.contains(network->ssid())) {
        QList<QByteArray> lines = listResult.split('\n');
        for (int i=1; i<lines.size(); ++i) {
            const QByteArray &line = lines.at(i);
            if (line.contains(network->ssid())) {
                int networkId = line.toInt();
                if (line.contains("[CURRENT]")) {
                    if (WIFI_DEBUG) qDebug("QWifiManager::connect(), network is already current");
                    handleConnected();
                } else {
                    if (WIFI_DEBUG) qDebug("QWifiManager::connect(), network known, but not enabled");
                    call(QByteArray("ENABLE_NETWORK ") + QByteArray::number(networkId));
                }
                return;
            }
        }
    }

    bool ok;
    QByteArray id = call("ADD_NETWORK").trimmed();
    id.toInt(&ok);
    if (!ok) {
        qWarning("QWifiManager::connect(), failed to add network");
        return;
    }
    QByteArray setNetworkCommand = QByteArray("SET_NETWORK ") + id;

    QByteArray ssidOK = call(setNetworkCommand + QByteArray(" ssid ") + '"' + network->ssid() + '"');
    QByteArray pskOK = call(setNetworkCommand + QByteArray(" psk ") + '"' + passphrase.toLatin1() + '"');

    if (ssidOK.trimmed() != QByteArray("OK") || pskOK.trimmed() != QByteArray("OK")) {
        call("REMOVE_NETWORK " + id);
        qWarning("QWifiManager::connect(), failed to set properties on network '%s'.\n'%s'\n'%s'",
                 id.constData(),
                 ssidOK.constData(),
                 pskOK.constData());
        return;
    }

    call(QByteArray("ENABLE_NETWORK ") + id);
}


class ProcessRunner : public QThread {
public:
    void run() {
        QProcess::execute(QStringLiteral("dhcpcd"), QStringList() << QStringLiteral("wlan0"));
        deleteLater();
    }
};


void QWifiManager::handleConnected()
{
    QList<QByteArray> lists = call("LIST_NETWORKS").split('\n');
    QByteArray connectedNetwork;
    for (int i=1; i<lists.size(); ++i) {
        if (lists.at(i).contains("[CURRENT]")) {
            connectedNetwork = lists.at(i);
            break;
        }
    }

    if (connectedNetwork.isEmpty()) {
        if (WIFI_DEBUG)
            qDebug("QWifiManager::handleConnected: not connected to a network...");
        m_online = false;
        onlineChanged(m_online);
        return;
    } else {
        if (WIFI_DEBUG)
            qDebug("QWifiManager::handleConnected: current is %s", connectedNetwork.constData());
    }

    QList<QByteArray> info = connectedNetwork.split('\t');
    m_connectedSSID = info.at(1);
    emit connectedSSIDChanged(m_connectedSSID);

    if (WIFI_DEBUG)
        qDebug("QWifiManager::handleConnected: starting dhcpcd...");
    QThread *t = new ProcessRunner();
    t->start();

    int ipaddr, gateway, mask, dns1, dns2, server, lease;
    if (do_dhcp_request(&ipaddr, &gateway, &mask, &dns1, &dns2, &server, &lease) == 0) {
        if (WIFI_DEBUG) {
            printf("ip ........: %s\n"
                   "gateway ...: %s\n"
                   "mask ......: %s\n"
                   "dns1 ......: %s\n"
                   "dns2 ......: %s\n"
                   "lease .....: %d\n",
                   int_to_ip(ipaddr).constData(),
                   int_to_ip(gateway).constData(),
                   int_to_ip(mask).constData(),
                   int_to_ip(dns1).constData(),
                   int_to_ip(dns2).constData(),
                   lease);
        }

        // Updating dns values..
        property_set("net.dns1", int_to_ip(dns1).constData());
        property_set("net.dns2", int_to_ip(dns2).constData());

        m_online = true;
        onlineChanged(false);

     } else {
        if (WIFI_DEBUG)
            qDebug("QWifiManager::handleConnected: dhcp request failed...");

     }
}
