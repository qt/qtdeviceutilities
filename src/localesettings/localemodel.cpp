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
#include <QLocale>
#include <QFuture>
#include <QFutureWatcher>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
#include "localemodel.h"

QT_BEGIN_NAMESPACE

/*!
    \class LocaleItem
    \inmodule QtLocaleSettings

    \brief The LocaleItem class represents a locale.

    This class holds the name, language, and country code of a locale.

    If available, the native country name and language are used. For example,
    \e Deutsch and \e Deutschland for the German locale.

    \sa QLocale, LocaleModel
*/

/*!
    \property LocaleItem::code
    \brief The locale code string.

    The locale code is in the format \e language_country, where \e language is
    a lowercase, two-letter ISO 639 language code, and \e country is an
    uppercase, two- or three-letter ISO 3166 country code.

    \sa QLocale::name()
*/

/*!
    \property LocaleItem::country
    \brief The name of the country.

    \sa QLocale::Country
*/

/*!
    \property LocaleItem::language
    \brief The name of the language.

    \sa QLocale::Language
*/


/*!
    Creates the locale item \a locale with the parent \a parent.
*/
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

/*!
    Returns the language of the country.
*/
QString LocaleItem::language() const
{
    return m_language;
}

/*!
    Returns the name of the country.
*/
QString LocaleItem::country() const
{
    return m_country;
}

/*!
    Returns the country code of the country.
*/
QString LocaleItem::code() const
{
    return m_code;
}

/*!
    \class LocaleModel
    \inmodule QtLocaleSettings

    \brief The LocaleModel class provides a model for the available locales.

    Each item in the model has a set of data elements associated with it, each
    with its own role. The roles are used by the view to indicate to the model
    which type of data it needs. Custom models should return data in these
    types.

    The data in a locale model can be filtered according to the country code,
    name, or language.

    \sa LocaleItem, LocaleFilterModel
*/

/*!
    \enum LocaleModel::Roles

    This enum holds the role of the locale item.

    For user roles, it is up to the developer to decide which types to use and
    ensure that components use the correct types when accessing and setting
    data.

    \value  Language
            The language of the country.
    \value  Country
            The name of the country.
    \value  Code
            The locale code string in the format \e language_country.

    \sa Qt::UserRole
*/

/*!
    \fn LocaleModel::addItem(LocaleItem* item)

    This signal is emitted when the locale item \a item is added to the locale
    model.
*/

/*!
    \fn LocaleModel::ready()

    This signal is emitted when the locale model has been reset.
*/

/*!
    Creates a locale model with the parent \a parent.
*/
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

/*!
    This signal is emitted when the locale model has been reset.
*/
void LocaleModel::modelReady()
{
    beginResetModel();
    sort(0);
    endResetModel();

    emit ready();
}

/*!
    Creates the locale model \a model.
*/
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

/*!
    Deletes the locale model.
*/
LocaleModel::~LocaleModel()
{
    qDeleteAll(m_items);
}

/*!
    Returns an array of user roles.

    \sa Roles
*/
QHash<int, QByteArray> LocaleModel::roleNames() const
{
    return m_roleNames;
}

/*!
    Returns the number of rows in the locale model that has \a parent.
*/
int LocaleModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

/*!
    Returns the locale item at \a index in the locale model for \a role.

    This item can be assigned to LocaleManager::locale(), when the user selects
    a locale from a list.

    \sa LocaleItem, Roles
*/
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

/*!
    Returns whether the locale item has more than one languages specified.

    If the language variant of the locale item if \a v1 is less than \a v2 ##?

*/
bool LocaleModel::variantLessThan(const LocaleItem* v1, const LocaleItem* v2)
{
    return v1->language() < v2->language();
}

/*!
    Sets the sorting order of the \a column items in the locale model to \a order.

    The sort order can be either \l {Qt::AscendingOrder}{ascending} or
    \l {Qt::DescendingOrder}{descending}.
*/
void LocaleModel::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED(column);
    Q_UNUSED(order);
    std::sort(m_items.begin(), m_items.end(), LocaleModel::variantLessThan);
}

/*!
    Returns the index for the country \a country in the locale model.

    The index is used by item views, delegates, and selection models to locate
    an item in the model.
*/
QModelIndex LocaleModel::indexForCountry(const QString &country) const
{
    for (int i = 0; i < m_items.count(); i++) {
        LocaleItem *item = m_items.at(i);
        if (item->country() == country ||
                item->language() == country) {
            return index(i);
        }
    }
    return QModelIndex();
}

QT_END_NAMESPACE
