/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
******************************************************************************/
#ifndef QNETWORKSETTINGSSERVICEMODEL_H
#define QNETWORKSETTINGSSERVICEMODEL_H

#include <QSortFilterProxyModel>
#include <QtNetworkSettings/qnetworksettings.h>

QT_BEGIN_NAMESPACE

class QNetworkSettingsService;
class Q_DECL_EXPORT QNetworkSettingsServiceModel : public QAbstractListModel
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

class Q_DECL_EXPORT QNetworkSettingsServiceFilter : public QSortFilterProxyModel
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
