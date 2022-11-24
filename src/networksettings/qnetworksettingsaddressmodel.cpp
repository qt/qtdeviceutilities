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
