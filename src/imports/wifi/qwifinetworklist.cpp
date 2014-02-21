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
#include "qwifinetworklist.h"

#include <QtCore>

const int ID_BSSID = (Qt::ItemDataRole) (Qt::UserRole + 1);
const int ID_SSID = (Qt::ItemDataRole) (Qt::UserRole + 2);
const int ID_SIGNAL = (Qt::ItemDataRole) (Qt::UserRole + 3);
const int ID_WPA2 = (Qt::ItemDataRole) (Qt::UserRole + 4);
const int ID_WPA = (Qt::ItemDataRole) (Qt::UserRole + 5);
const int ID_NETWORK = (Qt::ItemDataRole) (Qt::UserRole + 6);

QWifiNetworkList::QWifiNetworkList(QWifiManager *manager)
    : m_manager(manager)
{
}

QHash<int, QByteArray> QWifiNetworkList::roleNames() const
{
    QHash<int, QByteArray> names;
    names.insert(ID_BSSID, "bssid");
    names.insert(ID_SSID, "ssid");
    names.insert(ID_SIGNAL, "strength");
    names.insert(ID_WPA2, "wpa2");
    names.insert(ID_WPA, "wpa");
    names.insert(ID_NETWORK, "network");
    return names;
}

QVariant QWifiNetworkList::data(const QModelIndex &index, int role) const
{
    QWifiNetwork *n = m_networks.at(index.row());

    switch (role) {
    case ID_BSSID: return n->bssid();
    case ID_SSID: return n->ssid();
    case ID_SIGNAL: return n->signalStrength();
    case ID_WPA2: return n->supportsWPA2();
    case ID_WPA: return n->supportsWPA();
    case ID_NETWORK: return QVariant::fromValue((QObject *) n);
    }

    qWarning("QWifiNetworkList::data(), undefined role: %d\n", role);

    return QVariant();
}

QWifiNetwork *QWifiNetworkList::networkForSSID(const QByteArray &ssid, int *pos)
{
    for (int i=0; i<m_networks.size(); ++i) {
        if (m_networks.at(i)->ssid() == ssid) {
            if (pos)
                *pos = i;
            return m_networks.at(i);
        }
    }
    return 0;
}

void QWifiNetworkList::parseScanResults(const QByteArray &results)
{
    QList<QByteArray> lines = results.split('\n');

    QSet<QByteArray> sensibleNetworks;
    for (int i=1; i<lines.size(); ++i) {
        QList<QByteArray> info = lines.at(i).split('\t');
        if (info.size() < 5 || info.at(4).isEmpty() || info.at(0).isEmpty())
            continue;
        int pos = 0;
        if (!sensibleNetworks.contains(info.at(4)))
            sensibleNetworks.insert(info.at(4));
        QWifiNetwork *existingNetwork = networkForSSID(info.at(4), &pos);
        if (!existingNetwork) {
            QWifiNetwork *network = new QWifiNetwork();
            network->setBssid(info.at(0));
            network->setFlags(info.at(3));
            // signal strength is in dBm
            network->setSignalStrength(info.at(2).toInt());
            network->setSsid(info.at(4));
            beginInsertRows(QModelIndex(), m_networks.size(), m_networks.size());
            m_networks << network;
            endInsertRows();
        } else {
            // ssids are the same, compare bssids..
            if (existingNetwork->bssid() == info.at(0)) {
                // same access point, simply update the signal strength
                existingNetwork->setSignalStrength(info.at(2).toInt());
                dataChanged(createIndex(pos, 0), createIndex(pos, 0));
            } else if (existingNetwork->signalStrength() < info.at(2).toInt()) {
                // replace with a stronger access point within the same network
                m_networks.at(pos)->setBssid(info.at(0));
                m_networks.at(pos)->setFlags(info.at(3));
                m_networks.at(pos)->setSignalStrength(info.at(2).toInt());
                m_networks.at(pos)->setSsid(info.at(4));
                dataChanged(createIndex(pos, 0), createIndex(pos, 0));
            }
        }
    }
    // remove networks that have gone out of range
    for (int i = 0; i < m_networks.size(); ++i) {
        if (!sensibleNetworks.contains(m_networks.at(i)->ssid())) {
            beginRemoveRows(QModelIndex(), i, i);
            delete m_networks.takeAt(i);
            endRemoveRows();
        } else {
            ++i;
        }
    }
}


