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
#include "localemodel.h"
#include "localefiltermodel.h"

QT_BEGIN_NAMESPACE

/*!
    \class LocaleFilterModel
    \inmodule QtDeviceUtilities

    \brief The LocaleFilterModel class provides a filtered model for the
    available locales.

    This class can be used as the model in a view that lists the available
    locales.

    \sa LocaleModel
*/

/*!
    \property LocaleFilterModel::filter
    \brief Holds a string that filters out the locales in the model.

    The filtering process is a case-insensitive matching for whether the region
    (country) name contains this string. The string can be taken from user
    input.

    \sa LocaleItem::country
*/

/*!
    Creates a locale filer model with the parent \a parent.
*/
LocaleFilterModel::LocaleFilterModel(QObject* parent)
    :QSortFilterProxyModel(parent)
{
    connect(this, &LocaleFilterModel::filterChanged, this, &LocaleFilterModel::invalidate);
    LocaleModel *localeModel = new LocaleModel(this);
    setSourceModel(localeModel);
}

/*!
    Deletes the locale filter model.
*/
LocaleFilterModel::~LocaleFilterModel()
{

}

/*!
    Returns the locale filter string.
*/
QString LocaleFilterModel::filter() const
{
    return m_filter;
}

/*!
    Sets the locale filter string to \a aFilter.
*/
void LocaleFilterModel::setFilter(const QString& aFilter)
{
    m_filter = aFilter;
    emit filterChanged();
}

/*!
    Returns whether the row \a source_row has the country role and whether it is
    found in the locale model \a source_parent.
*/
bool LocaleFilterModel::filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const
{
    bool ret = false;
    if (this->sourceModel())
    {
       QModelIndex index = this->sourceModel()->index( source_row, 0, source_parent );
       if (index.isValid())
       {
           QVariant nameRole = index.data(LocaleModel::Country);
           if (nameRole.isValid())
           {
               QString name = nameRole.toString();
               if (name.contains(m_filter,  Qt::CaseInsensitive)) {
                   ret = true;
               }
           }
       }
    }
    return ret;
}

/*!
    Returns the locale item at \a row in the locale filter model.

    This item can be assigned to LocaleManager::locale(), when the user selects
    a locale from a list.

    \sa LocaleItem
*/
QVariant LocaleFilterModel::itemFromRow(const int row) const
{
    QModelIndex idx = index(row, 0);
    QModelIndex mapped = mapToSource(idx);
    if (mapped.isValid())
    {
        QVariant nameRole = mapped.data(LocaleModel::Code);
        if (nameRole.isValid())
        {
            return nameRole;
        }
    }
    return QVariant();
}

/*!
    Returns the index for the country \a country in the locale filter model.

    The index is used by item views, delegates, and selection models to locate
    an item in the model.
*/
int LocaleFilterModel::indexForCountry(const QString &country) const
{
    QAbstractItemModel *model = this->sourceModel();
    LocaleModel *localModel = qobject_cast<LocaleModel*>(model);
    QModelIndex i = localModel->indexForCountry(country);
    QModelIndex ret = mapFromSource(i);
    return ret.row();
}

QT_END_NAMESPACE
