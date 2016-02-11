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
#include "timezonemodel.h"
#include "timezonefiltermodel.h"

TimezoneFilterModel::TimezoneFilterModel(QObject* parent)
    :QSortFilterProxyModel(parent)
{
    connect(this, &TimezoneFilterModel::filterChanged, this, &TimezoneFilterModel::invalidate);
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
    return QVariant::fromValue(QString(""));
}

