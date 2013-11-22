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
#ifndef QWIFINETWORKLIST_H
#define QWIFINETWORKLIST_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QList>

#include "qwifinetwork.h"

class QWifiManager;

class QWifiNetworkList : public QAbstractListModel
{
    Q_OBJECT

public:

    QWifiNetworkList(QWifiManager *manager);

    void parseScanResults(const QByteArray &data);

    QWifiNetwork *networkForBSSID(const QByteArray &bssid, int *pos);

    int rowCount(const QModelIndex &) const { return m_networks.size(); }
    QVariant data(const QModelIndex &index, int role) const;

    QHash<int,QByteArray> roleNames() const;

private:
    QWifiManager *m_manager;
    QList<QWifiNetwork *> m_networks;
};

#endif // QWIFINETWORKLIST_H
