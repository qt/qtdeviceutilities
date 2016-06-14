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
#include "qnetworksettingsservicemodel.h"
#include "qnetworksettings.h"

QNetworkSettingsServiceModel::QNetworkSettingsServiceModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(Qt::UserRole, "modelData");
    m_roleNames.insert(Name, "name");
    m_roleNames.insert(SignalStrength, "signalStrength");
    m_roleNames.insert(Connected, "connected");
}

QNetworkSettingsServiceModel::~QNetworkSettingsServiceModel()
{

}

QHash<int, QByteArray> QNetworkSettingsServiceModel::roleNames() const
{
    return m_roleNames;
}


int QNetworkSettingsServiceModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

QVariant QNetworkSettingsServiceModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) return QVariant();

    QNetworkSettingsService *item = m_items[index.row()];
    if (role == Qt::UserRole) {
        return QVariant::fromValue(static_cast<QObject*>(item));
    }
    else if (role == Name) {
        return item->name();
    }
    else if (role == SignalStrength) {
        return item->wirelessConfig()->signalStrength();
    }
    else if (role == Connected) {
        return item->state() == QNetworkSettingsState::Online || item->state() == QNetworkSettingsState::Ready;
    }
    return QVariant();
}

void QNetworkSettingsServiceModel::append(QNetworkSettingsService* item)
{
    item->setParent(this);

    beginResetModel();
    m_items.append(item);
    endResetModel();
}

void QNetworkSettingsServiceModel::insert(int row, QNetworkSettingsService* item)
{
    item->setParent(this);

    beginInsertRows(QModelIndex(), row, row);
    m_items.insert(row, item);
    endInsertRows();
}

void QNetworkSettingsServiceModel::remove(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
}

bool QNetworkSettingsServiceModel::removeService(const QString &id)
{
    bool ret = false;
    for (int i=0; i < m_items.count(); i++) {
       if (m_items.at(i)->id() == id) {
           remove(i);
           ret = true;
           break;
       }
    }
    return ret;
}

void QNetworkSettingsServiceModel::updated(int row)
{
    dataChanged(createIndex(row, 0), createIndex(row, 0));
}

QList<QNetworkSettingsService*> QNetworkSettingsServiceModel::getModel()
{
    return m_items;
}

//Filter model

QNetworkSettingsServiceFilter::QNetworkSettingsServiceFilter(QObject* parent)
    :QSortFilterProxyModel(parent)
{
    connect(this, &QNetworkSettingsServiceFilter::typeChanged, this, &QNetworkSettingsServiceFilter::invalidate);
}

QNetworkSettingsServiceFilter::~QNetworkSettingsServiceFilter()
{

}

QNetworkSettingsType::Types  QNetworkSettingsServiceFilter::type() const
{
    return m_type;
}

void QNetworkSettingsServiceFilter::setType(const QNetworkSettingsType::Types type)
{
    m_type = type;
    emit typeChanged();
}

bool QNetworkSettingsServiceFilter::filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const
{
    if (this->sourceModel())
    {
       QModelIndex index = this->sourceModel()->index( source_row, 0, source_parent );
       if (index.isValid())
       {
           if (index.isValid())
           {
               QObject * obj = qvariant_cast<QObject *>(index.data(Qt::UserRole));
               QNetworkSettingsService * service = qobject_cast<QNetworkSettingsService *>(obj);
               if (service->type() == m_type)
                  return true;
           }
       }
    }
    return false;
}

QVariant QNetworkSettingsServiceFilter::itemFromRow(const int row) const
{
    QModelIndex idx = index(row, 0);
    QModelIndex mapped = mapToSource(idx);
    if (mapped.isValid())
    {
        QVariant serviceItem = mapped.data(Qt::UserRole);
        if (serviceItem.isValid())
        {
            return serviceItem;
        }
    }
    return QVariant::fromValue(QStringLiteral(""));
}

