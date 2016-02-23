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

#include <QLocale>
#include <QFuture>
#include <QFutureWatcher>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include "localemodel.h"


LocaleItem::LocaleItem(const QLocale& locale, QObject *parent)
    :QObject(parent)
{
    m_code = locale.name();
    m_country = locale.nativeCountryName();
    if (m_country.isEmpty()) {
        m_country = locale.countryToString(locale.country());
    }

    m_language = locale.nativeLanguageName();
    if (m_language.isEmpty()) {
        m_language = locale.languageToString(locale.language());
    }
}

QString LocaleItem::language() const
{
    return m_language;
}

QString LocaleItem::country() const
{
    return m_country;
}

QString LocaleItem::code() const
{
    return m_code;
}

LocaleModel::LocaleModel(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roleNames.insert(Qt::UserRole, "modelData");
    m_roleNames.insert(Country, "country");
    m_roleNames.insert(Language, "language");
    m_roleNames.insert(Code, "code");

    QFutureWatcher<void> *watcher = new QFutureWatcher<void>(this);
    QFuture<void> future = QtConcurrent::run(LocaleModel::generateModel, this);
    watcher->setFuture(future);
    connect(watcher, SIGNAL(finished()), this, SLOT(modelReady()));
}

void LocaleModel::modelReady()
{
    beginResetModel();
    sort(0);
    endResetModel();
}

void LocaleModel::generateModel(LocaleModel* model)
{
    QList<QLocale> allLocales = QLocale::matchingLocales(QLocale::AnyLanguage, QLocale::AnyScript, QLocale::AnyCountry);

    for (const QLocale &locale : allLocales) {
        if (locale.name() != QStringLiteral("C")) {
            LocaleItem *l = new LocaleItem(locale);
            l->moveToThread(model->thread());
            QMetaObject::invokeMethod(model, "addNewItem", Q_ARG( QObject*, qobject_cast<QObject*>(l)));
        }
    }
}

void LocaleModel::addNewItem(QObject *item)
{
   beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
   LocaleItem* newItem = qobject_cast<LocaleItem*>(item);
   if (newItem)
       m_items.append(newItem);
   endInsertRows();
}

LocaleModel::~LocaleModel()
{
    qDeleteAll(m_items);
}

QHash<int, QByteArray> LocaleModel::roleNames() const
{
    return m_roleNames;
}

int LocaleModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

QVariant LocaleModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) return QVariant();

    LocaleItem *item = m_items[index.row()];

    switch (role) {
    case Qt::UserRole:
        return QVariant::fromValue(static_cast<QObject*>(item));
        break;
    case Country:
        return item->country();
        break;
    case Language:
        return item->language();
        break;
    case Code:
        return item->code();
        break;
    default:
        return QVariant();
    }
}

bool LocaleModel::variantLessThan(const LocaleItem* v1, const LocaleItem* v2)
{
    return v1->language() < v2->language();
}

void LocaleModel::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED(column);
    Q_UNUSED(order);
    std::sort(m_items.begin(), m_items.end(), LocaleModel::variantLessThan);
}
