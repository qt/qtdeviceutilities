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
#include "plugin.h"
#include <qnetworksettings.h>
#include <qnetworksettingsmanager.h>
#include <qnetworksettingsservice.h>
#include <qnetworksettingsuseragent.h>
#include <qnetworksettingsservicemodel.h>
#include <qnetworksettingsinterfacemodel.h>

#include <qqml.h>
#include <QQmlEngine>
#include <QQmlContext>

/*!
    \qmlmodule QtDeviceUtilities.NetworkSettings 1.0

    \title Qt Device Utilities: Network Settings
    \ingroup qtee-qmlmodules
    \brief Provides singleton QML types for controlling network settings.

    Provides singleton QML types for controlling network settings in an
    embedded device.QAbstractItemModel

    Import the module as follows:

    \badcode
    import QtDeviceUtilities.NetworkSettings 1.0
    \endcode

    \l{Qt Device Utilities WiFi Tutorial} demonstrates how you can use
    \l{QtDeviceUtilities.NetworkSettings}{NetworkSettings} for connecting to WiFi.

    \note Some functions may not be available on all of the platforms.

    \section1 QML Types
*/

/*!
    \qmltype NetworkSettingsManager
    \inqmlmodule QtDeviceUtilities.NetworkSettings
    \brief A singleton QML type for managing network settings.

    There is no need to create an instance of this object. To use it,
    simply import the \c {QtDeviceUtilities.NetworkSettings} module.

    \l{Qt Device Utilities WiFi Tutorial} demonstrates how you can use
    \l{NetworkSettingsManager} for connecting to WiFi.
*/

/*!
    \qmlproperty model NetworkSettingsManager::services
    \readonly

    Holds the service model.

    The services list in the model can be controlled with the
    \c type property, and NetworkService items can be retrieved
    with the \c {itemFromRow(int index)} method. For example, to
    select the first available wired network service:

    \code
    property var service: null
    ...
    NetworkSettingsManager.services.type = NetworkSettingsType.Wired;
    service = NetworkSettingsManager.services.itemFromRow(0);
    \endcode

    Available service types:

    \value NetworkSettingsType.Wired     Wired network
    \value NetworkSettingsType.Wifi      Wifi network
    \value NetworkSettingsType.Bluetooth Bluetooth network
    \value NetworkSettingsType.Unknown   Unknown network type

    \sa NetworkService
*/

/*!
    \qmlproperty model NetworkSettingsManager::interfaces
    \readonly

    Holds the interface model. A delegate in a view that uses
    the \e interfaces model can access the NetworkInterface
    item with the \e dataModel role.

    \sa NetworkInterface
*/

/*!
    \qmlmethod NetworkService NetworkSettingsManager::service(string name, int type)

    Returns the service with name \a name and type \a type.

    \sa services
*/

/*!
    \qmlmethod NetworkSettingsManager::userAgent.setPassphrase(string passphrase)

    Sets the \a passphrase for connecting to a Wifi network.

    This method needs to be called in response to receiving a showUserCredentialsInput() signal.

    \sa userAgent.showUserCredentialsInput()
*/

/*!
    \qmlsignal NetworkSettingsManager::userAgent.showUserCredentialsInput()

    This signal is emitted when user credentials are required for connecting to a Wifi network.

    \sa userAgent.setPassphrase()
*/

/*!
    \qmlmethod NetworkSettingsManager::userAgent.cancelInput()

    Cancels the user credentials input request.
*/

/*!
    \qmlsignal NetworkSettingsManager::userAgent.error()

    This signal is emitted when the connection failed due to invalid user credentials.
*/

QT_BEGIN_NAMESPACE

template <typename T>
QObject *instance(QQmlEngine *engine, QJSEngine *) {
    T *t = new T(engine);
    t->setObjectName(T::staticMetaObject.className());
    return t;
}

void NetworksettingspluginPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(QLatin1String(uri) == QLatin1String("QtDeviceUtilities.NetworkSettings"));
    qmlRegisterUncreatableType<QNetworkSettingsService>(uri, 1, 0, "NetworkService", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsIPv4>(uri, 1, 0, "NetworkSettingsIPv4", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsIPv6>(uri, 1, 0, "NetworkSettingsIPv6", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsProxy>(uri, 1, 0, "NetworkSettingsProxy", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsType>(uri, 1, 0, "NetworkSettingsType", "Cannot be instantiated directly.");
    qmlRegisterUncreatableType<QNetworkSettingsState>(uri, 1, 0, "NetworkSettingsState", "Cannot be instantiated directly.");

    qRegisterMetaType<QNetworkSettingsUserAgent*>("QNetworkSettingsUserAgent*");
    qRegisterMetaType<QNetworkSettingsServiceFilter*>("QNetworkSettingsServiceFilter*");
    qRegisterMetaType<QNetworkSettingsInterfaceModel*>("QNetworkSettingsInterfaceModel*");

    qmlRegisterSingletonType<QNetworkSettingsManager>(uri, 1, 0, "NetworkSettingsManager", &instance<QNetworkSettingsManager>);
}

QT_END_NAMESPACE
