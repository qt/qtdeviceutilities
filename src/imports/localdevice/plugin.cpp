/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://www.qt.io
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://www.qt.io
**
****************************************************************************/
#include <qlocaldevice.h>
#include <QtQml>

/*!
    \qmlmodule QtDeviceUtilities.LocalDeviceSettings 1.0
    \title Qt Local Device QML Module
    \ingroup qtee-qmlmodules
    \brief A collection of the local device related utility functions, accessible from QML.
*/

/*!
    \page b2qt-addon-utils.html
    \title Qt Local Device Module
    \ingroup qtee-modules
    \brief A collection of the local device realted utility functions, accessible from QML.

    Provides utility functions for controlling an embedded
    device, such as device shutdown/reboot.

    Import the module as follows:

    \badcode
    import QtDeviceUtilities.LocalDeviceSettings 1.0
    \endcode

    This will give you access to the singleton QML type LocalDevice.

    \note Some functions are currently only implemented for one of
          the platforms.

    \section1 QML Types

    \annotatedlist utils-qmltypes
*/

/*!
    \qmltype LocalDevice
    \inqmlmodule QtDeviceUtilities.LocalDeviceSettings
    \ingroup utils-qmltypes
    \brief Singleton QML type providing access to utility functions.

    LocalDevice QML type is the interface to various utility
    functions.

    There is no need to create an instance of this object. To use it,
    simply import the \c {LocalDevice} module:

    \qml
    QtDeviceUtilities.LocalDeviceSettings

    \endqml

    \note Some functions are currently only implemented for one of
          the platforms.
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

#include "plugin.moc"
