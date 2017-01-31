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
#ifndef TIMEZONEMODEL_H
#define TIMEZONEMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QTimeZone>
#include <QMutex>

QT_BEGIN_NAMESPACE

class TimezoneItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString country READ country CONSTANT)
public:
    explicit TimezoneItem(const QByteArray& id, QObject *parent = Q_NULLPTR);
    QString name() const;
    QString country() const;
    QString id() const;

private:
    QString m_name;
    QString m_country;
    QString m_id;
};

class Q_DECL_EXPORT TimezoneModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TimezoneModel(QObject *parent = Q_NULLPTR);
    virtual ~TimezoneModel();
    // from QAbstractItemModel
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    enum Roles {
        Name = Qt::UserRole + 1,
        Country,
        Id
    };

    static void generateModel(TimezoneModel* model);

private Q_SLOTS:
    void addNewItem(QObject* item);

private:
    QList<TimezoneItem*> m_items;
    QHash<int, QByteArray> m_roleNames;
};

QT_END_NAMESPACE

#endif // TIMEZONEMODEL_H
