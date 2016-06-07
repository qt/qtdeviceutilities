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

LocaleFilterModel::LocaleFilterModel(QObject* parent)
    :QSortFilterProxyModel(parent)
{
    connect(this, &LocaleFilterModel::filterChanged, this, &LocaleFilterModel::invalidate);
    LocaleModel *localeModel = new LocaleModel(this);
    setSourceModel(localeModel);
}

LocaleFilterModel::~LocaleFilterModel()
{

}

QString LocaleFilterModel::filter() const
{
    return m_filter;
}

void LocaleFilterModel::setFilter(const QString& aFilter)
{
    m_filter = aFilter;
    emit filterChanged();
}

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

