// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QNETWORKSETTINGSINTERFACEMODEL_H
#define QNETWORKSETTINGSINTERFACEMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtNetworkSettings/qnetworksettingsglobal.h>

QT_BEGIN_NAMESPACE

class QNetworkSettingsInterface;
class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsInterfaceModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit QNetworkSettingsInterfaceModel(QObject *parent = nullptr);
    // from QAbstractItemModel
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void append(QNetworkSettingsInterface* networkInterface);
    void insert(int row, QNetworkSettingsInterface* networkInterface);
    void remove(int row);
    QList<QNetworkSettingsInterface*> getModel();

    enum Roles {
        Type = Qt::UserRole + 1,
        Status,
        Name,
        Powered
    };

    void updated(int row);
    bool removeInterface(const QString &name);
private Q_SLOTS:
    void connectionStatusChanged();
    void poweredChanged();

private:
    void connectStateChanges(QNetworkSettingsInterface* item);

private:
    QList<QNetworkSettingsInterface*> m_items;
    QHash<int, QByteArray> m_roleNames;
};

QT_END_NAMESPACE

#endif // QNETWORKSETTINGSINTERFACEMODEL_H
