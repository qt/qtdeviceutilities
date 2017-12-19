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
#include "timezonemodel.h"
#include "timezonefiltermodel.h"

QT_BEGIN_NAMESPACE

TimezoneFilterModel::TimezoneFilterModel(QObject* parent)
    :QSortFilterProxyModel(parent)
{
    connect(this, &TimezoneFilterModel::filterChanged, this, &TimezoneFilterModel::invalidate);
    TimezoneModel* timezoneModel = new TimezoneModel(this);
    setSourceModel(timezoneModel);
}

TimezoneFilterModel::~TimezoneFilterModel()
{

}

QString TimezoneFilterModel::filter() const
{
    return m_filter;
}

void TimezoneFilterModel::setFilter(const QString& aFilter)
{
    m_filter = aFilter;
    emit filterChanged();
}

bool TimezoneFilterModel::filterAcceptsRow( int source_row, const QModelIndex& source_parent ) const
{
    bool ret = false;
    if (this->sourceModel())
    {
       QModelIndex index = this->sourceModel()->index( source_row, 0, source_parent );
       if (index.isValid())
       {
           QVariant nameRole = index.data(TimezoneModel::Name);
           if (nameRole.isValid())
           {
               QString name = nameRole.toString();
               if (name.contains(m_filter,  Qt::CaseInsensitive)) {
                   ret = true;
               }
           }
           QVariant countryRole = index.data(TimezoneModel::Country);
           if (ret == false && countryRole.isValid())
           {
               QString country = countryRole.toString();
               if (country.contains(m_filter,  Qt::CaseInsensitive)) {
                   ret = true;
               }
           }
       }
    }
    return ret;
}

QVariant TimezoneFilterModel::itemFromRow(const int row) const
{
    QModelIndex idx = index(row, 0);
    QModelIndex mapped = mapToSource(idx);
    if (mapped.isValid())
    {
        QVariant nameRole = mapped.data(TimezoneModel::Name);
        if (nameRole.isValid())
        {
            return nameRole;
        }
    }
    return QVariant();
}

int TimezoneFilterModel::indexForTimezone(const QString &timezone) const
{
    TimezoneModel *model = qobject_cast<TimezoneModel *>(sourceModel());
    QList<TimezoneItem *> data = model->getModel();
    int row = 0;
    foreach (TimezoneItem *item, data) {
        if (item->id() == timezone) {
            QModelIndex mapped = mapFromSource(model->index(row));
            return mapped.row();
        }
        row++;
    }

    return -1;
}

QT_END_NAMESPACE
