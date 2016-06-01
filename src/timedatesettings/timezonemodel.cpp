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
#include <QLocale>
#include <QFuture>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include "timezonemodel.h"

TimezoneItem::TimezoneItem(const QByteArray& id, QObject *parent)
    :QObject(parent)
{
    QTimeZone tz = QTimeZone(id);
    m_name = tz.displayName(QTimeZone::StandardTime);
    m_country = QLocale::countryToString(tz.country());
    m_id = QString::fromUtf8(id);
}

QString TimezoneItem::name() const
{
    return m_name;
}

QString TimezoneItem::country() const
{
    return m_country;
}

QString TimezoneItem::id() const
{
    return m_id;
}

TimezoneModel::TimezoneModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(Qt::UserRole, "modelData");
    m_roleNames.insert(Country, "country");
    m_roleNames.insert(Name, "name");
    m_roleNames.insert(Id, "id");

    QFuture<void> t1 = QtConcurrent::run(TimezoneModel::generateModel, this);
}

void TimezoneModel::generateModel(TimezoneModel* model)
{
    QList<QByteArray> ids = QTimeZone::availableTimeZoneIds();
    foreach (QByteArray id, ids) {
        TimezoneItem *zone = new TimezoneItem(id);
        zone->moveToThread(model->thread());
        QMetaObject::invokeMethod(model, "addNewItem", Q_ARG( QObject*, qobject_cast<QObject*>(zone)));
    }
}

void TimezoneModel::addNewItem(QObject* item)
{
   beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
   TimezoneItem* newItem = qobject_cast<TimezoneItem*>(item);
   if (newItem)
       m_items.append(newItem);
   endInsertRows();
}

TimezoneModel::~TimezoneModel()
{
    qDeleteAll(m_items);
}

QHash<int, QByteArray> TimezoneModel::roleNames() const
{
    return m_roleNames;
}


int TimezoneModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

QVariant TimezoneModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) return QVariant();

    TimezoneItem *item = m_items[index.row()];

    switch (role) {
    case Qt::UserRole:
        return QVariant::fromValue(static_cast<QObject*>(item));
        break;
    case Name:
        return item->id();
        break;
    case Country:
        return item->country();
        break;
    case Id:
        return item->id();
        break;
    case Qt::DisplayRole:
        return item->id();
        break;
    default:
        return QVariant();
    }
}
