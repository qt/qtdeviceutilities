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
#include "qwifinetworklistmodel.h"

#include <QtCore>

const int ID_NETWORK = (Qt::ItemDataRole) (Qt::UserRole + 1);

QWifiNetworkListModel::QWifiNetworkListModel(QWifiManager *manager)
    : m_manager(manager)
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
    names.insert(ID_NETWORK, "network");
    return names;
}

QVariant QWifiNetworkListModel::data(const QModelIndex &index, int role) const
{
    QWifiNetwork *n = m_networks.at(index.row());
    switch (role) {
        case ID_NETWORK: return QVariant::fromValue((QObject *) n);
    }

    qWarning("QWifiNetworkListModel::data(), undefined role: %d\n", role);

    return QVariant();
}

QWifiNetwork *QWifiNetworkListModel::networkForSSID(const QByteArray &ssid, int *pos)
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

QWifiNetwork *QWifiNetworkListModel::outOfRangeListContains(const QByteArray &ssid)
{
    for (int i = 0; i < m_outOfRangeNetworks.length(); ++i)
        if (m_outOfRangeNetworks.at(i)->ssid() == ssid)
            return m_outOfRangeNetworks.takeAt(i);
    return 0;
}

void QWifiNetworkListModel::parseScanResults(const QByteArray &results)
{
    QList<QByteArray> lines = results.split('\n');
    QSet<QByteArray> sensibleNetworks;

    for (int i = 1; i < lines.size(); ++i) {
        QList<QByteArray> info = lines.at(i).split('\t');
        if (info.size() < 5 || info.at(4).isEmpty() || info.at(0).isEmpty())
            continue;
        int pos = 0;
        sensibleNetworks.insert(info.at(4));
        QWifiNetwork *knownNetwork = networkForSSID(info.at(4), &pos);
        if (!knownNetwork)
            knownNetwork = outOfRangeListContains(info.at(4));
        // signal strength is in dBm. Deprecated, but still widely used "wext"
        // wifi driver reports positive values for signal strength, we workaround that.
        int signalStrength = qAbs(info.at(2).trimmed().toInt()) * -1;
        if (!knownNetwork) {
            QWifiNetwork *network = new QWifiNetwork();
            network->setOutOfRange(false);
            network->setBssid(info.at(0));
            network->setFlags(info.at(3));
            network->setSignalStrength(signalStrength);
            network->setSsid(info.at(4));
            beginInsertRows(QModelIndex(), m_networks.size(), m_networks.size());
            m_networks << network;
            endInsertRows();
        } else {
            if (knownNetwork->outOfRange()) {
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
                m_networks.at(pos)->setSsid(info.at(4));
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
