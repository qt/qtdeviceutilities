/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
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
#include <cutils/properties.h>
#include <unistd.h>

#define WLAN_INTERFACE "wlan0"

static bool QT_WIFI_DEBUG = !qgetenv("QT_WIFI_DEBUG").isEmpty();

const QEvent::Type WIFI_SCAN_RESULTS = (QEvent::Type) (QEvent::User + 2001);
const QEvent::Type WIFI_CONNECTED = (QEvent::Type) (QEvent::User + 2002);

static int q_wifi_start_supplicant()
{
#if Q_ANDROID_VERSION_MAJOR > 4 || (Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR >= 1)
    return wifi_start_supplicant(0);
#else
    return wifi_start_supplicant();
#endif
}

static int q_wifi_connect_to_supplicant()
{
#if Q_ANDROID_VERSION_MAJOR > 4 || (Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR >= 1)
    return wifi_connect_to_supplicant(WLAN_INTERFACE);
#else
    return wifi_connect_to_supplicant();
#endif
}

static int q_wifi_command(const char *command, char *reply, size_t *reply_len)
{
#if Q_ANDROID_VERSION_MAJOR > 4 || (Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR >= 1)
    return wifi_command(WLAN_INTERFACE, command, reply, reply_len);
#else
    return wifi_command(command, reply, reply_len);
#endif

}

static int q_wifi_wait_for_event(char *buf, size_t len)
{
#if Q_ANDROID_VERSION_MAJOR > 4 || (Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR >= 1)
    return wifi_wait_for_event(WLAN_INTERFACE, buf, len);
#else
    return wifi_wait_for_event(buf, len);
#endif
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
    QWifiManagerEventThread(QWifiManager *manager)
        : m_manager(manager)
    {

    }

    void run() {
        if (QT_WIFI_DEBUG) qDebug("EventReceiver thread is running");
        char buffer[2048];
        while (1) {
            int size = q_wifi_wait_for_event(buffer, sizeof(buffer) - 1);
            if (size > 0) {
                buffer[size] = 0;

                if (QT_WIFI_DEBUG) qDebug("EVENT: %s", buffer);

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
    if (QT_WIFI_DEBUG) qDebug("QWifiManager: start");
    connectToBackend();
}



void QWifiManager::setScanning(bool scanning)
{
    if (m_scanning == scanning)
        return;

    m_scanning = scanning;
    emit scanningChanged(scanning);

    if (m_scanning) {
        if (QT_WIFI_DEBUG) qDebug("QWifiManager: scanning");
        call("SCAN");
        m_scanTimer = startTimer(5000);
    } else {
        if (QT_WIFI_DEBUG) qDebug("QWifiManager: stop scanning");
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
        m_internalState = IS_LoadDriver;

    if (m_internalState == IS_LoadDriver && (is_wifi_driver_loaded() || wifi_load_driver() >= 0))
        m_internalState = IS_StartBackend;

    if (m_internalState == IS_StartBackend && q_wifi_start_supplicant() >= 0) {
        m_internalState = IS_ConnectToBackend;
        sleep(3); //###
    }

    if (m_internalState == IS_ConnectToBackend && q_wifi_connect_to_supplicant() >= 0)
        m_internalState = IS_UpAndRunning;

    if (m_internalState == IS_UpAndRunning) {
        qDebug("QWifiManager: started successfully");

        emit readyChanged(true);
        m_eventThread = new QWifiManagerEventThread(this);
        m_eventThread->start();

        handleConnected();
    } else {
        qWarning("QWifiManager: stuck at internal state level: %d", m_internalState);
    }
}


QByteArray QWifiManager::call(const char *command)
{
    char data[2048];
    size_t len = sizeof(data) - 1;  // -1: room to add a 0-terminator
    if (q_wifi_command(command, data, &len) < 0) {
        qWarning("QWifiManager: call failed: %s", command);
        return QByteArray();
    }
    if (len < sizeof(data))
        data[len] = 0;
    QByteArray result = QByteArray::fromRawData(data, len);
    if (QT_WIFI_DEBUG) qDebug("QWifiManager::call: %s ==>\n%s", command, result.constData());
    return result;
}


bool QWifiManager::checkedCall(const char *command)
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
        //also possibly change online state
    }

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
        key_mgmt = "WPA_PSK";
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


class ProcessRunner : public QThread {
public:
    void run() {
        QStringList args;
        args << QStringLiteral("-A")
         << QStringLiteral("-h") << QStringLiteral("KAON")  //### hardcoded hostname, for testing
         << QStringLiteral("wlan0");
        QProcess::execute(QStringLiteral("dhcpcd"), args);
        deleteLater();
    }
};


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
        m_online = false;
        onlineChanged(m_online);
        return;
    } else {
        if (QT_WIFI_DEBUG) qDebug("QWifiManager::handleConnected: current is %s", connectedNetwork.constData());
    }

    QList<QByteArray> info = connectedNetwork.split('\t');
    m_connectedSSID = info.at(1);
    emit connectedSSIDChanged(m_connectedSSID);

    if (QT_WIFI_DEBUG) qDebug("QWifiManager::handleConnected: starting dhcpcd...");
    QThread *t = new ProcessRunner();
    t->start();

    int ipaddr, gateway, mask, dns1, dns2, server, lease;
    if (do_dhcp_request(&ipaddr, &gateway, &mask, &dns1, &dns2, &server, &lease) == 0) {
        if (QT_WIFI_DEBUG) {
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

        // Store (possibly updated) settings
        call("SAVE_CONFIG");

        m_online = true;
        onlineChanged(m_online);

     } else {
        if (QT_WIFI_DEBUG) qDebug("QWifiManager::handleConnected: dhcp request failed...");
     }
}
