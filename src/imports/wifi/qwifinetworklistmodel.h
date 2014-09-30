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
#ifndef QWIFINETWORKLISTMODEL_H
#define QWIFINETWORKLISTMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QList>

#include "qwifinetwork.h"

class QWifiManager;

class QWifiNetworkListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    QWifiNetworkListModel(QWifiManager *manager);
    ~QWifiNetworkListModel();

    void parseScanResults(const QByteArray &data);

    QWifiNetwork *networkForSSID(const QByteArray &ssid, int *pos);
    QWifiNetwork *outOfRangeListContains(const QByteArray &ssid);

    int rowCount(const QModelIndex &) const { return m_networks.size(); }
    QVariant data(const QModelIndex &index, int role) const;

    QHash<int,QByteArray> roleNames() const;

private:
    QWifiManager *m_manager;
    QList<QWifiNetwork *> m_networks;
    QList<QWifiNetwork *> m_outOfRangeNetworks;
};

#endif // QWIFINETWORKLISTMODEL_H
