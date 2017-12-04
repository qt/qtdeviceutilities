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
#include <qlocaldevice.h>
#include <QtQml>

/*!
    \qmlmodule QtDeviceUtilities.LocalDeviceSettings 1.0
    \title Qt Device Utilities: Local Device Settings
    \ingroup qtee-qmlmodules
    \brief A collection of local device related utility functions.

    Provides utility functions for controlling an embedded
    device, such as device shutdown/reboot.

    Import the module as follows:

    \badcode
    import QtDeviceUtilities.LocalDeviceSettings 1.0
    \endcode

    This will give you access to the singleton QML type LocalDevice.

    \note Some functions may not be available on all of the platforms.

    \section1 QML Types
*/

/*!
    \qmltype LocalDevice
    \inqmlmodule QtDeviceUtilities.LocalDeviceSettings
    \brief Singleton QML type providing access to utility functions.

    LocalDevice QML type is the interface to various utility
    functions.

    There is no need to create an instance of this object. To use it,
    simply import the \c QtDeviceUtilities.LocalDeviceSettings module:

    \badcode
    import QtDeviceUtilities.LocalDeviceSettings 1.0
    \endcode

    \note Some functions may not be available on all of the platforms.
*/

/*!
    \qmlmethod LocalDevice::reboot()

    Reboots the system. Does not return.

    \sa powerOff()
*/

/*!
    \qmlmethod LocalDevice::powerOff()

    Shuts down the system. Does not return.

    \sa reboot()
*/

QT_BEGIN_NAMESPACE

static QObject *module_api_factory(QQmlEngine *engine, QJSEngine *scriptEngine)
{
   Q_UNUSED(engine)
   Q_UNUSED(scriptEngine)
   QLocalDevice *api = new QLocalDevice();

   return api;
}

class B2QtUtilsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    B2QtUtilsPlugin()
    {
    }

    void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == "QtDeviceUtilities.LocalDeviceSettings");
        qmlRegisterSingletonType<QLocalDevice>(uri, 1, 0, "LocalDevice", module_api_factory);
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
