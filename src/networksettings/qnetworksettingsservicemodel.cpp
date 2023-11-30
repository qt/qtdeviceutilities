/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
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
#include "qnetworksettingsservice.h"
#include "qnetworksettings.h"

QT_BEGIN_NAMESPACE

/*!
    \class QNetworkSettingsServiceModel
    \inmodule QtNetworkSettings

    \brief The QNetworkSettingsServiceModel class represents a network service.

    The network service model contains a list of network services provided by
    the host.

    \sa QNetworkSettingsService
*/

/*!
    \enum QNetworkSettingsServiceModel::Roles

    This enum type holds information about the network connection.

    \value  Type
            Network \l{QNetworkSettingsType::Type}{type}.
    \value  Name
            The service set identifier (SSID) of the network.
    \value  SignalStrength
            The signal strength of the connection.
    \value  Connected
            Whether the connection has been established.
*/

/*!
    Creates a network service model with the parent \a parent.
*/
QNetworkSettingsServiceModel::QNetworkSettingsServiceModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(Qt::UserRole, "entry");
    m_roleNames.insert(Type, "type");
    m_roleNames.insert(Name, "name");
    m_roleNames.insert(SignalStrength, "signalStrength");
    m_roleNames.insert(Connected, "connected");
}

/*!
    Deletes the network service model.
*/
QNetworkSettingsServiceModel::~QNetworkSettingsServiceModel()
{

}

/*!
    Returns an array of the names of the roles in the model.
*/
QHash<int, QByteArray> QNetworkSettingsServiceModel::roleNames() const
{
    return m_roleNames;
}

/*!
    Returns the number of rows in the model with the parent \a parent.
*/
int QNetworkSettingsServiceModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

/*!
    Returns the data at the index \a index in the model for the type of data
    specified by \a role.
*/
QVariant QNetworkSettingsServiceModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) return QVariant();

    QNetworkSettingsService *item = m_items[index.row()];
    if (role == Qt::UserRole) {
        return QVariant::fromValue(static_cast<QObject*>(item));
    }
    else if (role == Type) {
        return item->type();
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

/*!
    Replaces placeholder data with \a item. Returns \c true on success.
*/
bool QNetworkSettingsServiceModel::replacePlaceholder(QNetworkSettingsService* item)
{
    if (item->type() == QNetworkSettingsType::Wired) {
        for (int i = 0; i < m_items.size(); ++i) {
            QNetworkSettingsService* existing = m_items.at(i);
            if (existing->placeholderState()) {
                m_items.replace(i, item);
                existing->deleteLater();
                updated(i);
                return true;
            }
        }
    }
    return false;
}

/*!
    Appends \a item to the model.
*/
void QNetworkSettingsServiceModel::append(QNetworkSettingsService* item)
{
    item->setParent(this);
    connectStateChanges(item);

    if ((item->type() == QNetworkSettingsType::Wired) && replacePlaceholder(item)) {
        return;
    }

    beginResetModel();
    m_items.append(item);
    endResetModel();
}

/*!
    Inserts \a item into \a row in the model.
*/
void QNetworkSettingsServiceModel::insert(int row, QNetworkSettingsService* item)
{
    item->setParent(this);
    connectStateChanges(item);

    if ((item->type() == QNetworkSettingsType::Wired) && replacePlaceholder(item)) {
        return;
    }

    beginInsertRows(QModelIndex(), row, row);
    m_items.insert(row, item);
    endInsertRows();
}

void QNetworkSettingsServiceModel::connectStateChanges(QNetworkSettingsService* item)
{
    connect(item, &QNetworkSettingsService::stateChanged, this, &QNetworkSettingsServiceModel::connectionStatusChanged);
    QNetworkSettingsWireless* wireless = item->wirelessConfig();
    if (wireless)
        connect(wireless, &QNetworkSettingsWireless::signalStrengthChanged, this, &QNetworkSettingsServiceModel::signalStrengthChanged);
}

/*!
    Removes the row \a row from the model.
*/
void QNetworkSettingsServiceModel::remove(int row)
{
    QNetworkSettingsService* item = m_items.at(row);
    if (item->type() == QNetworkSettingsType::Wired) {
        /* Don't remove a wired service so that it doesn't become undefined in the UI.
           This avoids problems when a cable is disconnected. */
        item->setPlaceholderState(true);
        return;
    }
    item->deleteLater();
    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
}

/*!
    Removes the service specified by \a id from the model. Returns \c true if the service
    was successfully removed, \c false otherwise.
*/
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

/*!
    Marks the data on the row \a row in the model as updated.
*/
void QNetworkSettingsServiceModel::updated(int row)
{
    dataChanged(createIndex(row, 0), createIndex(row, 0));
}

/*!
    Returns the service with the name \a name.
*/
QNetworkSettingsService* QNetworkSettingsServiceModel::getByName(const QString& name)
{
    QNetworkSettingsService* ret = nullptr;
    foreach (QNetworkSettingsService* item, m_items) {
        if (item->name() == name) {
            ret = item;
            break;
        }
    }
    return ret;
}

/*!
    Returns the network service model.
*/
QList<QNetworkSettingsService*> QNetworkSettingsServiceModel::getModel()
{
    return m_items;
}

/*!
    This signal is emitted when the connection status changes.
*/
void QNetworkSettingsServiceModel::connectionStatusChanged()
{
    QNetworkSettingsService *s = qobject_cast<QNetworkSettingsService*>(sender());

    int row = 0;
    foreach (QNetworkSettingsService* item, m_items) {
        if (item == s) {
            updated(row);
            break;
        }
        row++;
    }

}

/*!
    This signal is emitted when the signal strength changes.
*/
void QNetworkSettingsServiceModel::signalStrengthChanged()
{
    QNetworkSettingsWireless *s = qobject_cast<QNetworkSettingsWireless*>(sender());
    int row = 0;
    foreach (QNetworkSettingsService* item, m_items) {
        if (item->wirelessConfig() == s) {
            updated(row);
            break;
        }
        row++;
    }
}

//Filter model

/*!
    \class QNetworkSettingsServiceFilter
    \inmodule QtNetworkSettings

    \brief The QNetworkSettingsServiceFilter class represents a network service
    filter.

    \sa QNetworkSettingsService
*/

/*!
    \property QNetworkSettingsServiceFilter::type
    \brief The type of the network.

    \l QNetworkSettingsType::Type
*/

/*!
    \property QNetworkSettingsServiceFilter::wiredNetworksAvailable
    \brief Whether wired networks are available.
*/

/*!
    \fn QNetworkSettingsServiceFilter::typeChanged()

    This signal is emitted when the network type changes.
*/

/*!
    \fn QNetworkSettingsServiceFilter::wiredNetworksAvailableChanged()

    This signal is emitted when the availability of wired networks changes.
*/

/*!
    \qmltype NetworkSettingsServiceFilter
    \inqmlmodule QtDeviceUtilities.NetworkSettings
    \abstract
*/

/*!
    Creates a network service filter with the parent \a parent.
*/
QNetworkSettingsServiceFilter::QNetworkSettingsServiceFilter(QObject* parent)
    :QSortFilterProxyModel(parent), m_type(QNetworkSettingsType::Unknown)
{
    connect(this, &QNetworkSettingsServiceFilter::typeChanged, this, &QNetworkSettingsServiceFilter::invalidate);
}

/*!
    Deletes the network service filter.
*/
QNetworkSettingsServiceFilter::~QNetworkSettingsServiceFilter()
{

}

/*!
    \qmlproperty enumeration NetworkSettingsServiceFilter::type

    \value NetworkSettingsType.Wired Wired network
    \value NetworkSettingsType.Wifi Wifi network
    \value NetworkSettingsType.Bluetooth Bluetooth network
    \value NetworkSettingsType.Unknown Unknown network type
*/

/*!
    Returns the service model.

    \l QNetworkSettingsType::Type
*/
QNetworkSettingsType::Type  QNetworkSettingsServiceFilter::type() const
{
    return m_type;
}

/*!
    \fn void QNetworkSettingsServiceFilter::setType(QNetworkSettingsType::Type type)

    Sets the service model to \a type.
*/
void QNetworkSettingsServiceFilter::setType(const QNetworkSettingsType::Type type)
{
    m_type = type;
    emit typeChanged();
}

/*!
    Returns whether the row \a source_row has the user role and whether it is
    found in the model \a source_parent.
*/
bool QNetworkSettingsServiceFilter::filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const
{
    if (this->sourceModel())
    {
       QModelIndex index = this->sourceModel()->index( source_row, 0, source_parent );
       if (index.isValid())
       {
           QObject * obj = qvariant_cast<QObject *>(index.data(Qt::UserRole));
           QNetworkSettingsService * service = qobject_cast<QNetworkSettingsService *>(obj);
           if (service->type() == m_type || m_type == QNetworkSettingsType::Unknown)
              return true;
       }
    }
    return false;
}

/*!
    \qmlmethod NetworkService NetworkSettingsServiceFilter::itemFromRow(int index)

    Returns the service at \a index in the model.
*/

/*!
    Returns the service at \a row in the model.
*/
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

/*!
    \qmlmethod int NetworkSettingsServiceFilter::activeRow()

    Returns the connected service index in the model.
    Returns negative number if no active connection is available.
*/

/*!
    Returns the connected service index in the model.
    Returns negative number if no active connection is available.
*/
int QNetworkSettingsServiceFilter::activeRow() const
{
    QNetworkSettingsServiceModel* model = qobject_cast<QNetworkSettingsServiceModel*>(sourceModel());
    QList<QNetworkSettingsService*> data = model->getModel();
    int row = 0;
    foreach (QNetworkSettingsService* item, data) {
        if (item->type() == m_type &&
                (item->state() == QNetworkSettingsState::Ready ||
                 item->state() == QNetworkSettingsState::Online)) {
            QModelIndex idx = model->index(row, 0);
            QModelIndex mapped = mapFromSource(idx);
            return mapped.row();
        }
        row++;
    }
    return -1;
}

/*!
    Sets the availability of wired networks to \a wiredNetworksAvailable.
*/
void QNetworkSettingsServiceFilter::setWiredNetworksAvailable(bool wiredNetworksAvailable)
{
    m_wiredNetworksAvailable = wiredNetworksAvailable;
    emit wiredNetworksAvailableChanged();
}

QT_END_NAMESPACE
