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
#include <QProcess>
#include "qnetworksettingsmanager_p.h"
#include "qwificontroller_p.h"
#include "qnetworksettingsinterface_p.h"
#include "qnetworksettingsservice_p.h"
#include "qnetworksettingsuseragent.h"
#include "qwifisupplicant_p.h"

QNetworkSettingsManagerPrivate::QNetworkSettingsManagerPrivate(QNetworkSettingsManager *parent)
    :QObject(parent)
    ,q_ptr(parent)
{

    QNetworkSettingsUserAgent* userAgent = new QNetworkSettingsUserAgent(this);
    this->setUserAgent(userAgent);

    m_serviceModel = new QNetworkSettingsServiceModel(this);
    m_serviceFilter = new QNetworkSettingsServiceFilter(this);
    m_serviceFilter->setSourceModel(m_serviceModel);

    m_wifiController = new QWifiController(this);
    m_wifiController->asyncCall(QWifiController::InitializeBackend);

    QObject::connect(m_wifiController, &QWifiController::backendStateChanged,
                     this, &QNetworkSettingsManagerPrivate::handleBackendStateChanged);
    QObject::connect(m_wifiController, &QWifiController::dhcpRequestFinished,
                     this, &QNetworkSettingsManagerPrivate::handleDhcpRequestFinished);

    QObject::connect(m_wifiController, &QWifiController::raiseError, this, &QNetworkSettingsManagerPrivate::updateLastError);
    m_wifiController->start();

    updateWifiState();
}

QNetworkSettingsManagerPrivate::~QNetworkSettingsManagerPrivate()
{
    m_wifiController->asyncCall(QWifiController::ExitEventLoop);
    m_wifiController->wait();
    delete m_wifiController;
}

bool QNetworkSettingsManagerPrivate::event(QEvent *event)
{
    switch ((int) event->type()) {
    case WIFI_SCAN_RESULTS:
        parseScanResults(call(QStringLiteral("SCAN_RESULTS")));
        return true;
    case WIFI_CONNECTED:
        handleConnected();
        return true;
    case WIFI_DISCONNECTED:
        handleDisconneced();
        return true;
    case WIFI_AUTHENTICATING:
        handleAuthenticating(static_cast<QWifiEvent *>(event));
        return true;
    case WIFI_HANDSHAKE_FAILED:
        updateNetworkState(QNetworkSettingsState::Failure);
        return true;
    }
    return QObject::event(event);
}

void QNetworkSettingsManagerPrivate::connectNetwork(const QString& ssid)
{
    if (m_backendState != QWifiController::Running) {
        qCWarning(B2QT_WIFI) << "start wifi backend before calling connect()";
        return;
    }

    call(QStringLiteral("DISABLE_NETWORK all"));
    m_currentSSID = ssid;
    emit m_agent->showUserCredentialsInput();
}

void QNetworkSettingsManagerPrivate::userInteractionReady(bool cancel)
{
    if (cancel) {
        m_currentSSID = QStringLiteral("");
        return;
    }
    bool networkKnown = false;
    QString id;
    const QStringList configuredNetworks = call(QStringLiteral("LIST_NETWORKS")).split('\n');
    for (int i = 1; i < configuredNetworks.length(); ++i) {
        const QStringList networkFields = configuredNetworks.at(i).split('\t');
        const QString ssid = QWifiSupplicant::decodeSsid(networkFields.at(1));
        if (ssid == m_currentSSID) {
            id = networkFields.at(0);
            networkKnown = true;
            break;
        }
    }

    if (!networkKnown) {
        bool ok;
        id = call(QStringLiteral("ADD_NETWORK"));
        id.toInt(&ok);
        if (!ok) {
            updateLastError(QStringLiteral("failed to add network"));
            return;
        }
    }

    bool ok = true;
    QChar q = QLatin1Char('"');
    QString setNetworkCommand = QLatin1String("SET_NETWORK ") + id;
    if (!networkKnown) {
        ok = ok && checkedCall(setNetworkCommand + QLatin1String(" ssid ") + q + m_currentSSID + q);
    }

    QString key_mgmt;
    WpaSupplicantService *service = networkForSSID(m_currentSSID);
    if (!service) {
        return;
    }
    QString psk = m_agent->passPhrase();

    // --------------------- configure network ------------------------------
    // ref: http://w1.fi/cgit/hostap/plain/wpa_supplicant/wpa_supplicant.conf
    // ref: https://www.freebsd.org/cgi/man.cgi?wpa_supplicant.conf
    // ----------------------------------------------------------------------
    if (service->wirelessConfig()->supportsSecurity(QNetworkSettingsWireless::WPA) ||
               service->wirelessConfig()->supportsSecurity(QNetworkSettingsWireless::WPA2)) {
        // ### TODO - password length has limits (see IEEE 802.11), we need to check
        // for those limits here. Supplicant gives only a meaningless "fail" message.
        ok = ok && checkedCall(setNetworkCommand + QLatin1String(" psk ") + q + psk + q);
        key_mgmt = QLatin1String("WPA-PSK");
    } else if (service->wirelessConfig()->supportsSecurity(QNetworkSettingsWireless::WEP)) {
        ok = ok && checkedCall(setNetworkCommand + QLatin1String(" wep_key0 ") + q + psk + q);
        ok = ok && checkedCall(setNetworkCommand + QLatin1String(" auth_alg OPEN SHARED"));
        key_mgmt = QLatin1String("NONE");
    } else if (service->wirelessConfig()->supportsSecurity(QNetworkSettingsWireless::None)) {
        // open network
        key_mgmt = QLatin1String("NONE");
    }

    if (service->wirelessConfig()->hidden())
        ok = ok && checkedCall(setNetworkCommand + QLatin1String(" scan_ssid 1"));

    ok = ok && checkedCall(setNetworkCommand + QLatin1String(" key_mgmt ") + key_mgmt);
    if (!ok) {
        if (!networkKnown)
            call(QLatin1String("REMOVE_NETWORK ") + id);
        updateLastError(QLatin1String("failed to set properties on network: ") + id);
        return;
    }

    call(QLatin1String("SELECT_NETWORK ") + id);
    call(QStringLiteral("RECONNECT"));
}

void QNetworkSettingsManagerPrivate::disconnectNetwork()
{
    call(QStringLiteral("DISCONNECT"));
    m_wifiController->asyncCall(QWifiController::StopDhcp);
}

void QNetworkSettingsManagerPrivate::handleBackendStateChanged(QWifiController::BackendState backendState)
{
    switch (backendState) {
    case QWifiController::NotRunning:
        updateNetworkState(QNetworkSettingsState::Disconnect);
        break;
    default:
        break;
    }
    updateBackendState(backendState);
}

void QNetworkSettingsManagerPrivate::handleDhcpRequestFinished(const QString &status)
{
    qCDebug(B2QT_WIFI) << "handleDhcpRequestFinished: " << status << " for " << m_currentSSID;
    if (status == QLatin1String("success")) {
        updateNetworkState(QNetworkSettingsState::Online);
        call(QStringLiteral("SAVE_CONFIG"));
    } else {
        updateNetworkState(QNetworkSettingsState::Failure);
    }
}

void QNetworkSettingsManagerPrivate::setUserAgent(QNetworkSettingsUserAgent *agent)
{
    m_agent = agent;
    connect(m_agent, &QNetworkSettingsUserAgent::ready, this, &QNetworkSettingsManagerPrivate::userInteractionReady);
}

void QNetworkSettingsManagerPrivate::setCurrentSSID(const QString &ssid)
{
    qCDebug(B2QT_WIFI) << "current SSID: " << m_currentSSID << " -> " << ssid;
    if (m_currentSSID == ssid)
        return;

    m_currentSSID = ssid;
}

void QNetworkSettingsManagerPrivate::handleAuthenticating(QWifiEvent *event)
{
    QString data = event->data().trimmed();
    QString ssid = data.mid(data.indexOf(QLatin1String("SSID")) + 6);
    ssid = ssid.left(ssid.lastIndexOf(QLatin1Char('\'')));

    setCurrentSSID(QWifiSupplicant::decodeSsid(ssid));
    updateNetworkState(QNetworkSettingsState::Association);
}

void QNetworkSettingsManagerPrivate::handleConnected()
{
    qCDebug(B2QT_WIFI) << "connected network: " << m_currentSSID;
    updateNetworkState(QNetworkSettingsState::Ready);
    m_wifiController->asyncCall(QWifiController::AcquireIPAddress);
}

void QNetworkSettingsManagerPrivate::handleDisconneced()
{
    updateNetworkState(QNetworkSettingsState::Disconnect);
}

void QNetworkSettingsManagerPrivate::updateNetworkState(QNetworkSettingsState::States networkState)
{
    //Update interface
    if (!m_interfaceModel.getModel().isEmpty()) {
        WpaSupplicantSettingsInterface* interface = qobject_cast<WpaSupplicantSettingsInterface*>(m_interfaceModel.getModel().first());
        if (interface && interface->state() != networkState) {
            interface->setState(networkState);
        }
    }

    //Update service state
    WpaSupplicantService *service = networkForSSID(m_currentSSID);
    if (service) {
        service->setState(networkState);
    }
}

void QNetworkSettingsManagerPrivate::updateBackendState(QWifiController::BackendState backendState)
{
    if (m_backendState == backendState)
        return;

    m_backendState = backendState;

    if (m_backendState == QWifiController::Running && m_interfaceModel.getModel().isEmpty()) {
        WpaSupplicantSettingsInterface *interface = new WpaSupplicantSettingsInterface(this);
        m_interfaceModel.append(interface);
    } else if (m_backendState == QWifiController::NotRunning && m_interfaceModel.getModel().size() > 0){
        m_interfaceModel.remove(0);
    }
}

void QNetworkSettingsManagerPrivate::updateWifiState()
{
    QProcess ps;
    ps.start(QStringLiteral("ps"));
    if (!ps.waitForStarted()) {
        updateLastError(ps.program() + QLatin1String(": ") + ps.errorString());
        return;
    }

    ps.waitForFinished();
    bool supplicantRunning = ps.readAll().contains("wpa_supplicant");
    if (supplicantRunning && m_wifiController->resetSupplicantSocket())
        m_backendState = QWifiController::Running;
}

QString QNetworkSettingsManagerPrivate::call(const QString &command)
{
    if (m_backendState != QWifiController::Running)
        return QString();

    QByteArray reply;
    bool success = m_wifiController->supplicant()->sendCommand(command, &reply);
    if (!success) {
        qCDebug(B2QT_WIFI) << "call to supplicant failed";
        return QString();
    }

    return QLatin1String(reply.trimmed());
}

bool QNetworkSettingsManagerPrivate::checkedCall(const QString &command)
{
    return call(command).toUpper() == QLatin1String("OK");
}

void QNetworkSettingsManagerPrivate::updateLastError(const QString &error)
{
    qCWarning(B2QT_WIFI) << error;
    if (!m_interfaceModel.getModel().isEmpty()) {
        WpaSupplicantSettingsInterface* interface = qobject_cast<WpaSupplicantSettingsInterface*>(m_interfaceModel.getModel().first());
        if (interface) {
            interface->setState(QNetworkSettingsState::Failure);
        }
    }
}

WpaSupplicantService* QNetworkSettingsManagerPrivate::networkForSSID(const QString& ssid)
{
    int pos = 0;
    return networkForSSID(ssid, pos);
}

WpaSupplicantService* QNetworkSettingsManagerPrivate::networkForSSID(const QString& ssid, int& pos)
{
    QList<QNetworkSettingsService*> services = m_serviceModel->getModel();
    pos = 0;
    foreach (QNetworkSettingsService *service, services) {
        if (service->name() == ssid) {
            return qobject_cast<WpaSupplicantService*>(service);
        }
        pos++;
    }
    pos = -1;
    return NULL;
}

WpaSupplicantService* QNetworkSettingsManagerPrivate::outOfRangeListContains(const QString& ssid)
{
    QList<QNetworkSettingsService*> services = m_outOfRangeServiceModel.getModel();
    foreach (QNetworkSettingsService *service, services) {
        if (service->name() == ssid) {
            return qobject_cast<WpaSupplicantService*>(service);
        }
    }
    return NULL;
}

void QNetworkSettingsManagerPrivate::parseScanResults(const QString &results)
{
    QStringList lines = results.split('\n');
    QSet<QString> sensibleNetworks;

    for (int i = 1; i < lines.size(); ++i) {
        QStringList info = lines.at(i).split('\t');
        if (info.size() < 5 || info.at(4).isEmpty() || info.at(0).isEmpty())
            continue;
        int pos = 0;

        QString ssid = QWifiSupplicant::decodeSsid(info.at(4));
        if (ssid.isEmpty())
            continue;

        sensibleNetworks.insert(ssid);
        WpaSupplicantService *knownNetwork = networkForSSID(ssid, pos);

        if (!knownNetwork) {
            knownNetwork = outOfRangeListContains(ssid);
            m_outOfRangeServiceModel.getModel().removeOne(knownNetwork);
        }

        int signalStrength = info.at(2).trimmed().toInt();
        if (signalStrength < 0) {
            // signal is reported in dBm, rough conversion: best = -40, worst = -100
            int val = qAbs(qMax(-100, qMin(signalStrength, -40)) + 40); // clamp and normalize to 0
            signalStrength = 100 - (int) ((100.0 * (double) val) / 60.0);
        } else if (signalStrength > 100) {
            qCWarning(B2QT_WIFI) << "unexpected value for a signal level: " << signalStrength;
        }

        if (!knownNetwork) {
            WpaSupplicantService *network = new WpaSupplicantService(this);
            network->setId(info.at(0));
            network->setFlags(info.at(3));
            network->wirelessConfig()->setSignalStrength(signalStrength);
            network->setName(ssid);
            m_serviceModel->append(network);
        } else {
            if (knownNetwork->wirelessConfig()->outOfRange()) {
                // known network has come back into a range
                knownNetwork->wirelessConfig()->setOutOfRange(false);
                m_serviceModel->append(knownNetwork);
                pos = m_serviceModel->getModel().size() - 1;
            }
            // ssids are the same, compare bssids..
            if (knownNetwork->id() == info.at(0)) {
                // same access point, simply update the signal strength
                knownNetwork->wirelessConfig()->setSignalStrength(signalStrength);
                knownNetwork->wirelessConfig()->setOutOfRange(false);
                m_serviceModel->updated(pos);
            } else if (knownNetwork->wirelessConfig()->signalStrength() < signalStrength) {
                // replace with a stronger access point within the same network
                knownNetwork->wirelessConfig()->setOutOfRange(false);
                knownNetwork->setId(info.at(0));
                knownNetwork->setFlags(info.at(3));
                knownNetwork->wirelessConfig()->setSignalStrength(signalStrength);
                knownNetwork->setName(ssid);
                m_serviceModel->updated(pos);
            }
        }
    }
    // remove out-of-range networks from the data model
    QList<QNetworkSettingsService*> networks;
    for (int i = 0; i < networks.size();) {
        if (!sensibleNetworks.contains(networks.at(i)->name())) {
            WpaSupplicantService *n = qobject_cast<WpaSupplicantService*>(networks.at(i));
            m_serviceModel->remove(i);
            if (n) {
                n->wirelessConfig()->setOutOfRange(true);
                m_outOfRangeServiceModel.append(n);
            }
        } else {
            ++i;
        }
    }
}
