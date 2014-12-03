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
#include <b2qtdevice.h>
#include <QtQml>

/*!
    \qmlmodule B2Qt.Utils 1.0
    \title B2Qt Utils QML Module
    \ingroup qtee-qmlmodules
    \brief A collection of utility functions, accessible from QML.
*/

/*!
    \page b2qt-addon-utils.html
    \title B2Qt Utils Module
    \ingroup qtee-modules
    \brief A collection of utility functions, accessible from QML.

    Provides various utility functions for controlling an embedded
    device, such as display brightness, IP address and hostname, and
    device shutdown/reboot.

    Import the module as follows:

    \badcode
    import B2Qt.Utils 1.0
    \endcode

    This will give you access to the singleton QML type B2QtDevice.

    \note Some functions are currently only implemented for one of
          the platforms.

    \section1 QML Types

    \annotatedlist utils-qmltypes
*/

/*!
    \qmltype B2QtDevice
    \inqmlmodule B2Qt.Utils
    \ingroup utils-qmltypes
    \brief Singleton QML type providing access to utility functions.

    B2QtDevice QML type is the interface to various utility
    functions.

    There is no need to create an instance of this object. To use it,
    simply import the \c {B2Qt.Utils} module:

    \qml
    import B2Qt.Utils 1.0

    Text {
        text: qsTr("IP Address:") + B2QtDevice.ipAddress
    }
    \endqml

    \note Some functions are currently only implemented for one of
          the platforms.
*/

/*!
    \qmlmethod B2Qt.Utils::B2QtDevice::reboot()

    Reboots the system. Does not return.

    \sa powerOff()
*/

/*!
    \qmlmethod B2Qt.Utils::B2QtDevice::powerOff()

    Shuts down the system. Does not return.

    \sa reboot()
*/

/*!
    \qmlproperty int B2Qt.Utils::B2QtDevice::masterVolume

    This property holds the master volume of the device.
    The volume can range from \c 0 to \c 100 and is linear.
    Changing the master volume will affect all audio streams.

    \note Currently implemented only for \B2QA.
*/

/*!
    \qmlproperty int B2Qt.Utils::B2QtDevice::displayBrightness
    This property holds the display brightness (the intensity of the backlight).
    The value is in the range from \c 0 to \c 255, where 255 is the maximum
    brightness, and 0 is the minimum (typically, the backlight is turned off).

    \note Currently implemented only for \B2QA.
*/

/*!
    \qmlproperty string B2Qt.Utils::B2QtDevice::ipAddress
    \readonly

    This property holds the current IP address(es) of the device
    for all active network interfaces. If multiple IP addresses are defined,
    this property holds a comma-separated list. The localhost (loopback)
    IP addresses are omitted.

   \sa hostname
*/

/*!
    \qmlproperty string B2Qt.Utils::B2QtDevice::hostname

    This property holds the current hostname of the device.

   \sa ipAddress
*/

static QObject *module_api_factory(QQmlEngine *engine, QJSEngine *scriptEngine)
{
   Q_UNUSED(engine)
   Q_UNUSED(scriptEngine)
   B2QtDevice *api = new B2QtDevice();

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
        Q_ASSERT(QLatin1String(uri) == "B2Qt.Utils");
        qmlRegisterSingletonType<B2QtDevice>(uri, 1, 0, "B2QtDevice", module_api_factory);
    }
};

#include "plugin.moc"
