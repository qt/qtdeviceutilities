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
#include <QQmlEngine>
#include <qcoreapplication.h>
#include "displaysettings.h"

/*!
    \qmlmodule QtDeviceUtilities.DisplaySettings 1.0
    \title Qt Device Utilities: Display Settings
    \ingroup qtee-qmlmodules
    \brief Provides a singleton QML type for controlling display settings.

    Provides a singleton QML type for controlling display settings in an
    embedded device.

    Import the module as follows:

    \badcode
    import QtDeviceUtilities.DisplaySettings 1.0
    \endcode

    This will give you access to the singleton QML type DisplaySettings.

    \note Some functions may not be available on all of the platforms.

    \section1 QML Types
*/

/*!
    \qmltype DisplaySettings
    \inqmlmodule QtDeviceUtilities.DisplaySettings
    \brief A singleton QML type for controlling display settings.

    There is no need to create an instance of this object. To use it,
    simply import the \c {QtDeviceUtilities.DisplaySettings} module.
*/

/*!
    \qmlproperty int DisplaySettings::displayBrightness

    Holds the display brightness (intensity of the backlight). A valid
    range for the property is from 0 to 255, where 255 is the maximum
    brightness and 0 is the minimum (typically, the backlight turned off).
*/

/*!
    \qmlproperty int DisplaySettings::physicalScreenSizeInch

    Holds the physical (diagonal) screen size in inches.
*/

/*!
    \qmlproperty bool DisplaySettings::physicalScreenSizeOverride

    Holds whether the value in physicalScreenSizeInch should be
    used or not.
*/

QT_BEGIN_NAMESPACE

template <typename T>
QObject *instance(QQmlEngine *engine, QJSEngine *) {
    T *t = new T(engine);
    t->setObjectName(T::staticMetaObject.className());
    return t;
}

class DisplaySettingsQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri)
    {
        qmlRegisterType(QUrl("qrc:/Display.qml"), uri, 1, 0, "Display");
        qmlRegisterSingletonType<DisplaySettings>(uri, 1, 0, "DisplaySettings", &instance<DisplaySettings>);
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
