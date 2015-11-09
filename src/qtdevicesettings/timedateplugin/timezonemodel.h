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
#ifndef TIMEZONEMODEL_H
#define TIMEZONEMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QTimeZone>
#include <QMutex>

class TimezoneItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
public:
    explicit TimezoneItem(const QByteArray& id, QObject *parent=0);
    QString name() const;
    QString country() const;
    QString id() const;

private:
    QString m_name;
    QString m_country;
    QString m_id;
};

class TimezoneModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TimezoneModel(QObject *parent=0);
    virtual ~TimezoneModel();
    // from QAbstractItemModel
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE QVariant itemFromRow(const int row) const;

    enum Roles {
        Name = Qt::UserRole + 1,
        Country,
        Id
    };

    static void generateModel(TimezoneModel* model);

private slots:
    void addNewItem(QObject* item);

private:
    QList<TimezoneItem*> m_items;
    QHash<int, QByteArray> m_roleNames;
};

#endif // TIMEZONEMODEL_H
