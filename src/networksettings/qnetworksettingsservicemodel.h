/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef QNETWORKSETTINGSSERVICEMODEL_H
#define QNETWORKSETTINGSSERVICEMODEL_H

#include <QSortFilterProxyModel>
#include "qnetworksettings.h"
#include "qnetworksettingsservice.h"

class Q_DECL_EXPORT QNetworkSettingsServiceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit QNetworkSettingsServiceModel(QObject *parent = Q_NULLPTR);
    virtual ~QNetworkSettingsServiceModel();
    // from QAbstractItemModel
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    void append(QNetworkSettingsService* networkService);
    void insert(int row, QNetworkSettingsService* networkInterface);
    void remove(int row);
    bool removeService(const QString &id);
    void updated(int row);
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

class QNetworkSettingsServiceFilter : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QNetworkSettingsType::Types type READ type WRITE setType NOTIFY typeChanged)
public:
    explicit QNetworkSettingsServiceFilter(QObject* parent = Q_NULLPTR);
    virtual ~QNetworkSettingsServiceFilter();
    bool filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const override;
    QNetworkSettingsType::Types type() const;
    void setType(QNetworkSettingsType::Types type);
    Q_INVOKABLE QVariant itemFromRow(const int row) const;
    Q_INVOKABLE int activeRow() const;
Q_SIGNALS:
    void typeChanged();
private:
    QNetworkSettingsType::Types m_type;
};

#endif // QNETWORKSETTINGSSERVICEMODEL_H
