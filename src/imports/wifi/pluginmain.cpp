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
#include <B2QtWifi/QWifiManager>
#include <B2QtWifi/QWifiDevice>
#include <B2QtWifi/QWifiConfiguration>

#include <QtQml>

QT_BEGIN_NAMESPACE

/*!
    \qmlmodule B2Qt.Wifi 1.0
    \title B2Qt Wifi QML Module
    \ingroup qtee-qmlmodules
    \brief A module for managing wireless network connectivity.

*/

/*!
    \page b2qt-addon-wifi.html
    \title B2Qt Wifi Module
    \ingroup qtee-modules
    \brief A module for managing wireless network connectivity.

    B2Qt.Wifi provides QML types and C++ classes for:

    \list
    \li Wifi backend handling
    \li Retrieval of information from Wifi network access points
    \li Wifi connection handling
    \li System's network interface configuration
    \li Other tasks that allow the device to access network
    \endlist

    \section1 Writing the Code

    Depending on what fits best with the application, you can choose to use
    either C++ or QML for accessing Wifi functionality. If the majority of the
    user interface is written in QML, it is recommended that you use the QML
    types, which provide a simpler and declarative way to handle Wifi networks.

    \list
    \li \l{Getting Started with B2Qt.Wifi in QML}
    \li \l{Getting Started with B2Qt.Wifi in C++}
    \endlist

    \section1 API Reference

    \section2 QML Types

    \annotatedlist wifi-qmltypes

    \section2 C++ Classes

    \annotatedlist wifi-cppclasses
*/

/*!
    \module B2Qt.Wifi.Cpp 1.0
    \title B2Qt Wifi C++ Module
    \ingroup qtee-cppmodules
    \brief A module for managing wireless network connectivity.
*/

/*!
    \qmltype WifiManager
    \inqmlmodule B2Qt.Wifi
    \ingroup wifi-qmltypes
    \brief Main interface to the Wifi functionality.

    WifiManager is a singleton type that is a central point for handling Wifi functionality
    related tasks. You can use WifiManager's API to control the Wifi backend, scan surroundings for
    Wifi access points and connect a device to a wireless network.

    When scanning for networks, WifiManager packs the results in a list-based data model which can
    be used with Qt's Model/View classes. Information about the Wifi networks is supplied via the model's
    interface, accessed with the following roles:

    \list
    \li \e ssid - informal (human) name of a Wifi network (string)
    \li \e bssid - basic service set identification of a network, used to uniquely identify BSS (string)
    \li \e signalStrength - strength of a Wifi signal represented as percentage (0-100) (int)
    \li \e supportsWPA - holds whether network access point supports WPA security protocol (bool)
    \li \e supportsWPA2 - holds whether network access point supports WPA2 security protocol (bool)
    \li \e supportsWEP - holds whether network access point supports WEP security protocol (bool)
    \li \e supportsWPS - holds whether network access point supports WPS security protocol (bool)
    \endlist
 */

/*!
    \qmlproperty enumeration WifiManager::NetworkState
    \readonly

    This property holds the current state of the network connection.

    \list
    \li \e WifiManager.Disconnected - Not connected to any network
    \li \e WifiManager.Authenticating - Verifying password with the network provider
    \li \e WifiManager.HandshakeFailed - Incorrect password provided
    \li \e WifiManager.ObtainingIPAddress - Requesting IP address from DHCP server
    \li \e WifiManager.DhcpRequestFailed - Could not retrieve IP address
    \li \e WifiManager.Connected - Ready to process network requests
    \endlist
*/

/*!
    \qmlproperty enumeration WifiManager::BackendState
    \readonly

    This property holds the current state of the Wifi backend.

    \list
    \li \e WifiManager.Initializing - Wireless supplicant is starting up
    \li \e WifiManager.Running - Supplicant is initialized and ready to process commands
    \li \e WifiManager.Terminating - Shutting down wireless supplicant
    \li \e WifiManager.NotRunning - Wireless supplicant process is not running
    \endlist
*/

/*!
    \qmlsignal WifiManager::networkStateChanged(NetworkState networkState)

    This signal is emitted whenever changes in a network state occur. The network name for
    which the NetworkState change events are send can be obtained from currentSSID.

    \sa NetworkState, currentSSID
*/

/*!
    \qmlsignal WifiManager::backendStateChanged(BackendState backendState)

    This signal is emitted whenever changes in a backend state occur.

    \sa start, stop
*/

/*!
    \qmlsignal WifiManager::currentSSIDChanged(string currentSSID)

    This signal is emitted when switching between different Wifi networks.

    \sa start, stop
*/

/*!
    \qmlsignal WifiManager::scanningChanged(bool scanning)

    This signal is emitted when device starts or stops to scan for available Wifi networks.

    \sa scanning
*/

/*!
    \qmlsignal WifiManager::lastErrorChanged(string error)

    This signal is emitted if some internal process has failed, \a error contains
    a message on what has failed.

    \sa connect
*/

/*!
    \qmlproperty model WifiManager::networks
    \readonly

    This property holds a list-based data model of networks that can be sensed by a device.
    Model can be used with Qt's Model/View classes like ListView. Data in the model is updated
    every 5 seconds if scanning is enabled.

    \sa scanning
*/

/*!
    \qmlproperty string WifiManager::currentSSID
    \readonly

    This property holds a network name of last selected network, the network for
    which the NetworkState change events are sent. Property can contain an empty
    string when no active network connection exists.
*/

/*!
    \qmlmethod WifiManager::start()

    Start the Wifi backend. This function returns immediately, the BackendState
    change events are delivered asynchronously.

    \sa stop, BackendState
*/

/*!
    \qmlmethod WifiManager::stop()

    Stop the Wifi backend and if connected to any network shut down the network connection.
    This function returns immediately, the BackendState change events are delivered asynchronously.

    \sa start, BackendState
*/

/*!
    \qmlproperty bool WifiManager::scanning

    This property holds whether or not the backend is scanning for Wifi networks. To
    preserve battery energy, set this property to false when scanning is not required.
    When enabled, new readings are taken every 5 seconds.

    For scanning to work, first you need to initialize the Wifi backend.

    \sa start
*/

/*!
    \qmlproperty string WifiManager::lastError
    \readonly

    This property holds an error message if some internal process has failed.

    \sa connect
*/

/*!
    \qmlmethod bool WifiManager::connect(WifiConfiguration config)

    Connect a device to a network using the \a config network configuration.
    This method returns \a true if the network with provoded configuration could be
    successfully added by the backend or \a false on failure, to obtain an error message
    read lastError property.

    \sa disconnect, NetworkState, lastError
*/

/*!
    \qmlmethod WifiManager::disconnect()

    Disconnect from currently connected network connection.

    \sa connect, NetworkState
*/

/*!
    \qmltype WifiDevice
    \inqmlmodule B2Qt.Wifi
    \ingroup wifi-qmltypes
    \brief Represents a physical device.

    Use this element to query if a device is Wifi capable, before attempting
    to use the functionality of WifiManager.

    \qml
    import B2Qt.Wifi 1.0

    GroupBox {
        id: wifiOptions
        title: "Wifi"
        visible: false

        Component.onCompleted: {
            if (WifiDevice.wifiSupported()) {
                var component = Qt.createComponent("WifiGroupBoxContent.qml")
                var wifi = component.createObject(wifiOptions.contentItem)
                if (wifi)
                    wifiOptions.visible = true
            } else {
                print("Wifi functionality not available on this device.")
            }
        }
    }
    \endqml
*/

/*!
    \qmlmethod bool WifiDevice::wifiSupported()

    Returns \a true if a device is Wifi capable - Wifi driver and firmware has been
    successfully loaded by the system, otherwise returns \a false.
*/

/*!
    \qmltype WifiConfiguration
    \inqmlmodule B2Qt.Wifi
    \ingroup wifi-qmltypes
    \brief Used to define a network configuration.

    WifiConfiguration object represents a single network configuration. Use this object
    to configure a properties of your network, for example what passphrase and security
    protocol to use. WifiManager's connect() function takes this object and connects a
    device to a network that matches the provided configuration.
*/

/*!
    \qmlproperty string WifiConfiguration::ssid

    This property holds informal (human) name of a Wifi network.
*/

/*!
    \qmlproperty string WifiConfiguration::passphrase

    This property holds the passphrase to use for authenticating with a network.
*/

/*!
    \qmlproperty string WifiConfiguration::protocol

    This property holds the security protocols to use for Wifi connection.
    WPA is used by default if property is not set, supported values are: WPA, WPA2, WEP, WPS.
*/

/*!
    \qmlproperty bool WifiConfiguration::ssidHidden

    If a Wifi access point does not broadcast its SSID, setting this property
    to \c true ensures that the Wifi backend can detect the specified network.

    By default this property is set to \c false.
*/

static QObject *globalWifiDevice(QQmlEngine *, QJSEngine *)
{
    return new QWifiDevice;
}

static QObject *globalWifiManager(QQmlEngine *, QJSEngine *)
{
    return QWifiManager::instance();
}

class QWifiPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")

public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("B2Qt.Wifi"));

        qmlRegisterType<QAbstractListModel>();
        qmlRegisterSingletonType<QWifiManager>(uri, 1, 0, "WifiManager", globalWifiManager);
        qmlRegisterSingletonType<QWifiDevice>(uri, 1, 0, "WifiDevice", globalWifiDevice);
        qmlRegisterType<QWifiConfiguration>(uri, 1, 0, "WifiConfiguration");
    }
};

QT_END_NAMESPACE

#include "pluginmain.moc"
