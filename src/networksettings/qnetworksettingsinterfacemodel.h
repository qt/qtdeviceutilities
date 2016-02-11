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
#ifndef QNETWORKSETTINGSINTERFACEMODEL_H
#define QNETWORKSETTINGSINTERFACEMODEL_H

#include <QAbstractListModel>
#include "qnetworksettings.h"

class QNetworkSettingsInterface;

class Q_DECL_EXPORT QNetworkSettingsInterfaceModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit QNetworkSettingsInterfaceModel(QObject *parent=0);
    // from QAbstractItemModel
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    void append(QNetworkSettingsInterface* networkInterface);
    void insert(int row, QNetworkSettingsInterface* networkInterface);
    void remove(int row);
    QList<QNetworkSettingsInterface*> getModel();

    enum Roles {
        Type = Qt::UserRole + 1,
        Status,
        Name
    };

private:
    QList<QNetworkSettingsInterface*> m_items;
    QHash<int, QByteArray> m_roleNames;
};

#endif // QNETWORKSETTINGSINTERFACEMODEL_H
