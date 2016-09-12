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

#include <systemtime.h>
#include "timezonefiltermodel.h"
/*!
    \qmlmodule QtDeviceUtilities.TimeDateSettings 1.0

    \title Qt Device Utilities: Time and Date Settings
    \ingroup qtee-qmlmodules
    \brief Provides singleton QML types for controlling date and time
           settings.

    Provides singleton QML types for controlling date and time settings
    in an embedded device.

    Import the module as follows:

    \badcode
    import QtDeviceUtilities.TimeDateSettings 1.0
    \endcode

    \note Some functions may not be available on all of the platforms.

    \section1 QML Types
*/

/*!
    \qmltype TimeManager
    \inqmlmodule QtDeviceUtilities.TimeDateSettings
    \brief A singleton QML type for managing the system date and time.

    There is no need to create an instance of this object. To use it,
    simply import the \c {QtDeviceUtilities.TimeDateSettings} module.
*/

/*!
    \qmlproperty string TimeManager::timeZone

    Holds the current time zone string.
*/

/*!
    \qmlproperty bool TimeManager::ntp

    Holds whether the system time is synchronized using NTP (Network
    Time Protocol).
*/

/*!
    \qmlproperty Date TimeManager::time

    Holds the current date and time.
*/

/*!
    \qmltype TimezonesFilter
    \inqmlmodule QtDeviceUtilities.TimeDateSettings
    \brief Provides a filtered model for the available time zones.

    There is no need to create an instance of this object. To use it,
    simply import the \c {QtDeviceUtilities.TimeDateSettings} module.

    The TimezonesFilter QML type can be used as the model in a view
    that lists the available time zones.

    For example:

    \code
    ListView {
        model: TimezonesFilter
        delegate: Text { text: name + " | " + country }
    }
    \endcode

    Available \e roles in the time zone model:

    \table
    \header \li Role \li Description

    \row \li \c name
         \li Display name of the time zone. See QTimeZone::displayName() for
             details.

    \row \li \c country
         \li Name of the country for the zone time.

    \row \li \c id
         \li IANA time zone ID.
    \endtable
*/

/*!
    \qmlproperty string TimezonesFilter::filter

    Holds a string that filters out the time zones in the model.
    The filtering process is a case-insensitive match for
    whether the time zone's name or country name contain the
    \e filter substring; it can be taken from user input.
*/

/*!
    \qmlmethod object TimezonesFilter::itemFromRow(int index)

    Returns the item at \a index in the model. This item can
    be assigned to \l [QML] {TimeManager::timeZone}
    {TimeManager.timeZone}, when the user selects a time zone
    from a list.
*/
template <typename T>
QObject *instance(QQmlEngine *engine, QJSEngine *) {
    T *t = new T(engine);
    t->setObjectName(T::staticMetaObject.className());
    return t;
}

class TimeDateSettingsQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri)
    {
        qmlRegisterSingletonType<SystemTime>(uri, 1, 0, "TimeManager", &instance<SystemTime>);
        qmlRegisterSingletonType<TimezoneFilterModel>(uri, 1, 0, "TimezonesFilter", &instance<TimezoneFilterModel>);
    }
};

#include "plugin.moc"
