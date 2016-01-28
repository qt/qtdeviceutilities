/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <QQmlPropertyMap>
#include "qnetworksettingsinterfacemodel.h"
#include "qnetworksettingsinterface.h"

QNetworkSettingsInterfaceModel::QNetworkSettingsInterfaceModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(Qt::UserRole, "modelData");
    m_roleNames.insert(Type, "type");
    m_roleNames.insert(Status, "status");
    m_roleNames.insert(Name, "name");
}

QNetworkSettingsInterfaceModel::~QNetworkSettingsInterfaceModel()
{

}

QHash<int, QByteArray> QNetworkSettingsInterfaceModel::roleNames() const
{
    return m_roleNames;
}


int QNetworkSettingsInterfaceModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

QVariant QNetworkSettingsInterfaceModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) return QVariant();

    QNetworkSettingsInterface *item = m_items[index.row()];
    if (role == Qt::UserRole) {
        return QVariant::fromValue(static_cast<QObject*>(item));
    }

    switch (role) {
    case Type:
        return item->type();
        break;
    case Name:
        return item->name();
        break;
    case Status:
        return item->state();
        break;
    default:
        return "";
    }

}

void QNetworkSettingsInterfaceModel::append(QNetworkSettingsInterface* item)
{
    item->setParent(this);

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_items.append(item);
    endInsertRows();
}

void QNetworkSettingsInterfaceModel::insert(int row, QNetworkSettingsInterface* item)
{
    item->setParent(this);

    beginInsertRows(QModelIndex(), row, row);
    m_items.insert(row, item);
    endInsertRows();
}

QList<QNetworkSettingsInterface*> QNetworkSettingsInterfaceModel::getModel()
{
    return m_items;
}
