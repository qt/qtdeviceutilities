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
#include <QtCore/QVariant>
#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QLoggingCategory>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(B2QT_WIFI)

class QWifiNetwork;

class QWifiNetworkListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit QWifiNetworkListModel(QObject *parent = 0);
    virtual ~QWifiNetworkListModel();

    int rowCount(const QModelIndex &) const { return m_networks.size(); }
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int,QByteArray> roleNames() const;

    void parseScanResults(const QString &data);
    QWifiNetwork *networkForSSID(const QString &ssid);
    QWifiNetwork *networkForSSID(const QString &ssid, int *pos);
    QWifiNetwork *outOfRangeListContains(const QString &ssid);

private:
    QList<QWifiNetwork *> m_networks;
    QList<QWifiNetwork *> m_outOfRangeNetworks;
};

QT_END_NAMESPACE

#endif // QWIFINETWORKLISTMODEL_H
