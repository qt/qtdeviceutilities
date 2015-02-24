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
#include "qwifinetworklistmodel_p.h"
#include "qwifinetwork_p.h"
#include "qwifiutils_p.h"

#include "qwifimanager.h"

#include <QtCore/QSet>
#include <QtCore/QString>
#include <QtCore/QByteArray>

QT_BEGIN_NAMESPACE

QWifiNetworkListModel::QWifiNetworkListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QWifiNetworkListModel::~QWifiNetworkListModel()
{
    qDeleteAll(m_networks);
    qDeleteAll(m_outOfRangeNetworks);
    m_networks.clear();
    m_outOfRangeNetworks.clear();
}

QHash<int, QByteArray> QWifiNetworkListModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names.insert(QWifiManager::SSID, "ssid");
    names.insert(QWifiManager::BSSID, "bssid");
    names.insert(QWifiManager::SignalStrength, "signalStrength");
    names.insert(QWifiManager::WPASupported, "supportsWPA");
    names.insert(QWifiManager::WPA2Supported, "supportsWPA2");
    names.insert(QWifiManager::WEPSupported, "supportsWEP");
    names.insert(QWifiManager::WPSSupported, "supportsWPS");
    return names;
}

QVariant QWifiNetworkListModel::data(const QModelIndex &index, int role) const
{
    QWifiNetwork *n = m_networks.at(index.row());

    switch (role) {
    case QWifiManager::SSID:
        return n->ssid();
        break;
    case QWifiManager::BSSID:
        return n->bssid();
        break;
    case QWifiManager::SignalStrength:
        return n->signalStrength();
        break;
    case QWifiManager::WPASupported:
        return n->supportsWPA();
        break;
    case QWifiManager::WPA2Supported:
        return n->supportsWPA2();
        break;
    case QWifiManager::WEPSupported:
        return n->supportsWEP();
        break;
    case QWifiManager::WPSSupported:
        return n->supportsWPS();
        break;
    default:
        break;
    }

    return QVariant();
}

QWifiNetwork *QWifiNetworkListModel::networkForSSID(const QString &ssid)
{
    int pos = 0; // unused
    return networkForSSID(ssid, &pos);
}

QWifiNetwork *QWifiNetworkListModel::networkForSSID(const QString &ssid, int *pos)
{
    for (int i = 0; i < m_networks.size(); ++i) {
        if (m_networks.at(i)->ssid() == ssid) {
            if (pos)
                *pos = i;
            return m_networks.at(i);
        }
    }
    return 0;
}

QWifiNetwork *QWifiNetworkListModel::outOfRangeListContains(const QString &ssid)
{
    for (int i = 0; i < m_outOfRangeNetworks.length(); ++i)
        if (m_outOfRangeNetworks.at(i)->ssid() == ssid)
            return m_outOfRangeNetworks.takeAt(i);
    return 0;
}

void QWifiNetworkListModel::parseScanResults(const QString &results)
{
    QStringList lines = results.split('\n');
    QSet<QString> sensibleNetworks;

    for (int i = 1; i < lines.size(); ++i) {
        QStringList info = lines.at(i).split('\t');
        if (info.size() < 5 || info.at(4).isEmpty() || info.at(0).isEmpty())
            continue;
        int pos = 0;

        QString ssid = QWifiUtils::decodeHexEncoded(info.at(4));
        if (ssid.isEmpty())
            continue;

        sensibleNetworks.insert(ssid);
        QWifiNetwork *knownNetwork = networkForSSID(ssid, &pos);
        if (!knownNetwork)
            knownNetwork = outOfRangeListContains(ssid);

        int signalStrength = info.at(2).trimmed().toInt();
        if (signalStrength < 0) {
            // signal is reported in dBm, rough conversion: best = -40, worst = -100
            int val = qAbs(qMax(-100, qMin(signalStrength, -40)) + 40); // clamp and normalize to 0
            signalStrength = 100 - (int) ((100.0 * (double) val) / 60.0);
        } else if (signalStrength > 100) {
            qCWarning(B2QT_WIFI) << "unexpected value for a signal level: " << signalStrength;
        }

        if (!knownNetwork) {
            QWifiNetwork *network = new QWifiNetwork();
            network->setOutOfRange(false);
            network->setBssid(info.at(0));
            network->setFlags(info.at(3));
            network->setSignalStrength(signalStrength);
            network->setSsid(ssid);
            beginInsertRows(QModelIndex(), m_networks.size(), m_networks.size());
            m_networks << network;
            endInsertRows();
        } else {
            if (knownNetwork->isOutOfRange()) {
                // known network has come back into a range
                knownNetwork->setOutOfRange(false);
                beginInsertRows(QModelIndex(), m_networks.size(), m_networks.size());
                m_networks << knownNetwork;
                endInsertRows();
                pos = m_networks.length() - 1;
            }
            // ssids are the same, compare bssids..
            if (knownNetwork->bssid() == info.at(0)) {
                // same access point, simply update the signal strength
                knownNetwork->setSignalStrength(signalStrength);
                knownNetwork->setOutOfRange(false);
                dataChanged(createIndex(pos, 0), createIndex(pos, 0));
            } else if (knownNetwork->signalStrength() < signalStrength) {
                // replace with a stronger access point within the same network
                m_networks.at(pos)->setOutOfRange(false);
                m_networks.at(pos)->setBssid(info.at(0));
                m_networks.at(pos)->setFlags(info.at(3));
                m_networks.at(pos)->setSignalStrength(signalStrength);
                m_networks.at(pos)->setSsid(ssid);
                dataChanged(createIndex(pos, 0), createIndex(pos, 0));
            }
        }
    }
    // remove out-of-range networks from the data model
    for (int i = 0; i < m_networks.size();) {
        if (!sensibleNetworks.contains(m_networks.at(i)->ssid())) {
            beginRemoveRows(QModelIndex(), i, i);
            QWifiNetwork *n = m_networks.takeAt(i);
            n->setOutOfRange(true);
            m_outOfRangeNetworks.append(n);
            endRemoveRows();
        } else {
            ++i;
        }
    }
}

QT_END_NAMESPACE
