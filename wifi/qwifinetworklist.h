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
