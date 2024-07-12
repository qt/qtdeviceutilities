// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QNETWORKSETTINGSSERVICEMODEL_H
#define QNETWORKSETTINGSSERVICEMODEL_H

#include <QtCore/qsortfilterproxymodel.h>
#include <QtNetworkSettings/qnetworksettings.h>

QT_BEGIN_NAMESPACE

class QNetworkSettingsService;
class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsServiceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit QNetworkSettingsServiceModel(QObject *parent = nullptr);
    virtual ~QNetworkSettingsServiceModel();
    // from QAbstractItemModel
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool replacePlaceholder(QNetworkSettingsService* networkService);
    void append(QNetworkSettingsService* networkService);
    void insert(int row, QNetworkSettingsService* networkInterface);
    void remove(int row);
    bool removeService(const QString &id);
    void updated(int row);
    QNetworkSettingsService* getByName(const QString& name);
    QList<QNetworkSettingsService*> getModel();

    enum Roles {
        Type = Qt::UserRole + 1,
        Name,
        SignalStrength,
        Connected
    };


private Q_SLOTS:
    void connectionStatusChanged();
    void signalStrengthChanged();

private:
    void connectStateChanges(QNetworkSettingsService* item);

    QList<QNetworkSettingsService*> m_items;
    QHash<int, QByteArray> m_roleNames;
};

class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsServiceFilter : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QNetworkSettingsType::Type type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(bool wiredNetworksAvailable MEMBER m_wiredNetworksAvailable WRITE setWiredNetworksAvailable NOTIFY wiredNetworksAvailableChanged)
public:
    explicit QNetworkSettingsServiceFilter(QObject* parent = nullptr);
    virtual ~QNetworkSettingsServiceFilter();
    bool filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const override;
    QNetworkSettingsType::Type type() const;
    void setType(QNetworkSettingsType::Type type);
    Q_INVOKABLE QVariant itemFromRow(const int row) const;
    Q_INVOKABLE int activeRow() const;
    void setWiredNetworksAvailable(bool);
Q_SIGNALS:
    void typeChanged();
    void wiredNetworksAvailableChanged();
private:
    QNetworkSettingsType::Type m_type;
    bool m_wiredNetworksAvailable;
};

QT_END_NAMESPACE

#endif // QNETWORKSETTINGSSERVICEMODEL_H
