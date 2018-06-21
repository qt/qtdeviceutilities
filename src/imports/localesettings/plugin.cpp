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
#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/qqml.h>
#include <qcoreapplication.h>
#include <QQmlEngine>
#include <QQmlContext>

#include <systemlocale.h>
#include "localefiltermodel.h"

/*!
    \qmlmodule QtDeviceUtilities.LocaleSettings 1.0

    \title Qt Device Utilities: Locale Settings
    \ingroup qtee-qmlmodules
    \brief Provides singleton QML types for controlling locale settings.

    Provides singleton QML types for controlling locale settings in an
    embedded device.

    Import the module as follows:

    \badcode
    import QtDeviceUtilities.LocaleSettings 1.0
    \endcode

    \note Some functions may not be available on all of the platforms.

    \section1 QML Types
*/

/*!
    \qmltype LocaleManager
    \inqmlmodule QtDeviceUtilities.LocaleSettings
    \brief A singleton QML type for managing the system locale.

    There is no need to create an instance of this object. To use it,
    simply import the \c {QtDeviceUtilities.LocaleSettings} module.

    The \l locale property holds the current system locale string.
    Pass it to \l {QtQml::Qt::locale()}{Qt.locale}() to
    retrieve locale-specific properties.

    For example:

    \qml
    import QtQuick 2.6
    import QtDeviceUtilities.LocaleSettings 1.0

    Item {
        property var currentLocale: Qt.locale(LocaleManager.locale)
        Text { text: currentLocale.nativeLanguageName }
    }
    \endqml

    \sa LocaleFilter, {QtQml::}{Locale}
*/

/*!
    \qmlproperty string LocaleManager::locale

    Holds the system locale string in the format \e language_country,
    for example, "en_US".

    \sa QLocale::name()
*/

/*!
    \qmltype LocaleFilter
    \inqmlmodule QtDeviceUtilities.LocaleSettings
    \brief Provides a filtered model for the available locales.

    There is no need to create an instance of this object. To use it,
    simply import the \c {QtDeviceUtilities.LocaleSettings} module.

    The LocaleFilter QML type can be used as the model in a view
    that lists the available locales.

    For example:

    \code
    ListView {
        model: LocaleFilter
        delegate: Text { text: language + " | " + country }
    }
    \endcode

    Available \e roles in the locale model:

    \table
    \header \li Role \li Description

    \row \li \c code \li Locale code string in the format \e language_country.
                                See QLocale::name() for details.

    \row \li \c country \li The name of the country. If available, the native
                            country name is used.

    \row \li \c language \li The name of the language. If available, the
                             native language name is used.
    \endtable

    \sa LocaleManager
*/

/*!
    \qmlproperty string LocaleFilter::filter

    Holds a string that filters out the locales in the model.
    The filtering process is a case-insensitive match for
    whether the region (country) name contains the \e filter
    substring; it can be taken from user input.
*/

/*!
    \qmlmethod object LocaleFilter::itemFromRow(int index)

    Returns the item at \a index in the model. This item can
    assigned to \l [QML] {LocaleManager::locale}
    {LocaleManager.locale}, when the user selects a locale
    from a list.
*/

QT_BEGIN_NAMESPACE

template <typename T>
QObject *instance(QQmlEngine *engine, QJSEngine *) {
    T *t = new T(engine);
    t->setObjectName(T::staticMetaObject.className());
    return t;
}

class LocaleSettingsQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri)
    {
        qmlRegisterSingletonType<SystemLocale>(uri, 1, 0, "LocaleManager", &instance<SystemLocale>);
        qmlRegisterSingletonType<LocaleFilterModel>(uri, 1, 0, "LocaleFilter", &instance<LocaleFilterModel>);
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
