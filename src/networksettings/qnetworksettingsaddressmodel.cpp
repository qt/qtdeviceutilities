// Copyright (C) 2019 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include "qnetworksettingsaddressmodel.h"

QT_BEGIN_NAMESPACE

/*!
    \class QNetworkSettingsAddressModel
    \inmodule QtNetworkSettings

    \brief The QNetworkSettingsAddressModel class represents a network
    interface address.

    An address model contains a lists of available network interface addresses.

    \sa QNetworkSettingsManager
*/

/*!
    \property QNetworkSettingsAddressModel::count
    \brief Holds the number of rows in the model.
*/

/*!
    \fn QNetworkSettingsAddressModel::countChanged()
    This signal is emitted when the number of rows in the model changes.
*/

/*!
    Creates an address model with the parent \a parent.
*/
QNetworkSettingsAddressModel::QNetworkSettingsAddressModel(QObject *parent)
    :QStringListModel(parent)
{
}

/*!
    Creates an address model with the addresses specified by
    \a strings and parent \a parent.
*/
QNetworkSettingsAddressModel::QNetworkSettingsAddressModel(const QStringList &strings, QObject *parent)
    :QStringListModel(parent)
{
    setStringList(strings);
}

/*!
    Adds the addresses specified by \a addresses to the address model.
*/
void QNetworkSettingsAddressModel::setStringList(const QStringList &addresses)
{
    m_addresses = addresses;
    QStringListModel::setStringList(m_addresses);
    emit countChanged();
}

/*!
    Adds the address specified by \a address to the address model.
*/
void QNetworkSettingsAddressModel::append(const QString& address)
{
    int row = rowCount();

    bool succeed = insertRows(row, 1);
    if (succeed)
        succeed = setData(index(row), QVariant::fromValue(address));

    Q_ASSERT(succeed == true);

    emit countChanged();
}

/*!
    Removes the address at the position specified by \a index from the address
    model.
*/
void QNetworkSettingsAddressModel::remove(int index)
{
    removeRows(index, 1);
    emit countChanged();
}

/*!
    Returns the number of rows in the address model.
*/
int QNetworkSettingsAddressModel::count() const
{
    return rowCount();
}

/*!
    Resets the changes made to the addresses in the address model.
*/
void QNetworkSettingsAddressModel::resetChanges()
{
    QStringListModel::setStringList(m_addresses);
}

QT_END_NAMESPACE
