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
#include <QFutureWatcher>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include "timezonemodel.h"

QT_BEGIN_NAMESPACE

/*!
    \class TimezoneItem
    \inmodule QtDeviceUtilities
    \brief The TimezoneItem class represents a time zone.
*/

/*!
    \property TimezoneItem::id
    \brief Holds the IANA time zone ID.

    \sa QTimeZone::id(), {IANA Time Zone IDs}
*/

/*!
    \property TimezoneItem::name
    \brief Holds the localized display name of the time zone.

    \sa QTimeZone::displayName()
*/

/*!
    \property TimezoneItem::country
    \brief Holds the name of the country for the time zone.

    \sa QTimeZone::country()
*/

/*!
    Creates a time zone item with the IANA time zone ID \a id and parent
    \a parent.
*/
TimezoneItem::TimezoneItem(const QByteArray& id, QObject *parent)
    :QObject(parent)
{
    QTimeZone tz = QTimeZone(id);
    m_name = tz.displayName(QTimeZone::StandardTime);
    m_country = QLocale::countryToString(tz.country());
    m_id = QString::fromUtf8(id);
}

/*!
    Returns the localized display name of the time zone.

    \sa QTimeZone::displayName()
*/
QString TimezoneItem::name() const
{
    return m_name;
}

/*!
    Returns the name of the country for the time zone.

    \sa QTimeZone::country()
*/
QString TimezoneItem::country() const
{
    return m_country;
}

/*!
    Returns the IANA time zone ID.

    \sa QTimeZone::id(), {IANA Time Zone IDs}
*/
QString TimezoneItem::id() const
{
    return m_id;
}

/*!
    \class TimezoneModel
    \inmodule QtDeviceUtilities
    \brief The TimezoneModel class provides a model for the available time
    zones.

    This class can be used as the model in a view that lists the available time
    zones.
*/

/*!
    \enum TimezoneModel::Roles

    This enum type holds information about a time zone.

    \value  Name
            Localized display name of the time zone
    \value  Country
            Name of the country for the time zone
    \value  Id
            IANA time zone ID
*/

/*!
    \fn QList<TimezoneItem *> TimezoneModel::getModel() const

    Returns the time zone model for a time zone item.
*/

/*!
    \fn TimezoneModel::ready()

    This signal is emitted when the model is ready.
*/

/*!
    Creates a new time zone model with the parent \a parent.
*/
TimezoneModel::TimezoneModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(Qt::UserRole, "modelData");
    m_roleNames.insert(Country, "country");
    m_roleNames.insert(Name, "name");
    m_roleNames.insert(Id, "id");


    QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
    QFuture<void> future = QtConcurrent::run(TimezoneModel::generateModel, this);
    watcher->setFuture(future);
    connect(watcher, SIGNAL(finished()), this, SIGNAL(ready()));
}

/*!
    Creates the time zone model \a model.
*/
void TimezoneModel::generateModel(TimezoneModel* model)
{
    QList<QByteArray> ids = QTimeZone::availableTimeZoneIds();
    foreach (QByteArray id, ids) {
        TimezoneItem *zone = new TimezoneItem(id);
        zone->moveToThread(model->thread());
        QMetaObject::invokeMethod(model, "addNewItem", Q_ARG( QObject*, qobject_cast<QObject*>(zone)));
    }
}

/*!
    Adds the time zone item \a item to the time zone model.
*/
void TimezoneModel::addNewItem(QObject* item)
{
   beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
   TimezoneItem* newItem = qobject_cast<TimezoneItem*>(item);
   if (newItem)
       m_items.append(newItem);
   endInsertRows();
}

/*!
    Deletes the time zone model.
*/
TimezoneModel::~TimezoneModel()
{
    qDeleteAll(m_items);
}

/*!
    Returns an array of the names of the roles in the model.
*/
QHash<int, QByteArray> TimezoneModel::roleNames() const
{
    return m_roleNames;
}

/*!
    Returns the number of rows in the model with the parent \a parent.
*/
int TimezoneModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

/*!
    Returns the data at the index \a index in the model for the type of data
    specified by \a role.
*/
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

QT_END_NAMESPACE
