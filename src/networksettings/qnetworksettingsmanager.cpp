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
#include "qnetworksettingsmanager.h"
#include "qnetworksettingsservice.h"
#include "qnetworksettingsservicemodel.h"
#include "qnetworksettingsinterface.h"
#include "qnetworksettingsinterfacemodel.h"
#include "qnetworksettingsmanager_p.h"
#include "qnetworksettingsuseragent.h"
#include <QStringListModel>

QT_BEGIN_NAMESPACE

/*!
    \class QNetworkSettingsManager
    \inmodule QtNetworkSettings

    \brief The QNetworkSettingsManager class manages network settings.

    The network manager is designed to be used as a model that contains lists
    of available network interfaces and services.

    A delegate in a view that uses the interface model can access the
    network interface item with the data model role.

    The services list in the model can be controlled with the
    \l QNetworkSettingsType::type property, and network service items can be
    retrieved with the \l QNetworkSettingsServiceFilter::itemFromRow() method.

    \sa QNetworkSettingsService
*/

/*!
    \property QNetworkSettingsManager::services
    \brief Holds the service model.

    \sa QNetworkSettingsServiceFilter
*/

/*!
    \property QNetworkSettingsManager::interfaces
    \brief Holds the interface model.

    \sa QNetworkSettingsInterfaceModel
*/

/*!
    \property QNetworkSettingsManager::userAgent
    \brief Holds the user credentials for connecting to a network.

    \sa QNetworkSettingsUserAgent
*/

/*!
    \property QNetworkSettingsManager::currentWifiConnection
    \brief Holds the current Wifi connection.

    \sa QNetworkSettingsService
*/

/*!
    \property QNetworkSettingsManager::currentWiredConnection
    \brief Holds the current wired connection.

    \sa QNetworkSettingsService
*/

/*!
    \fn QNetworkSettingsManager::servicesChanged()

    This signal is emitted when the network service changes.
*/

/*!
    \fn QNetworkSettingsManager::interfacesChanged()

    This signal is emitted when the network interface changes.
*/

/*!
    \fn QNetworkSettingsManager::currentWifiConnectionChanged()

    This signal is emitted when the current Wifi connection changes.
*/

/*!
    \fn QNetworkSettingsManager::currentWiredConnectionChanged()

    This signal is emitted when the current wired connection changes.
*/

/*!
    Creates a network manager with the parent \a parent.
*/
QNetworkSettingsManager::QNetworkSettingsManager(QObject *parent) :
   QObject(parent)
   ,d_ptr(new QNetworkSettingsManagerPrivate(this))
{
}

/*!
    Returns the service model.

    \l QNetworkSettingsType::Types
*/
QNetworkSettingsServiceFilter *QNetworkSettingsManager::services()
{
    Q_D(QNetworkSettingsManager);
    return d->serviceFilter();
}

/*!
    Returns the interface model.
*/
QNetworkSettingsInterfaceModel *QNetworkSettingsManager::interfaces()
{
    Q_D(QNetworkSettingsManager);
    return d->interfaceModel();
}

/*!
    Returns the service model \a name of the type \a type.
*/
QNetworkSettingsService* QNetworkSettingsManager::service(const QString& name, int type)
{
    Q_D(QNetworkSettingsManager);

    foreach (QNetworkSettingsService* service, d->serviceModel()->getModel()) {
        if (service->name() == name && service->type() == type) {
            return service;
        }
    }
    return nullptr;
}

/*!
    Creates a connection to the network specified by \a name using the password
    \a passphrase.
*/
void QNetworkSettingsManager::connectBySsid(const QString &name, const QString &passphrase)
{
    Q_D(QNetworkSettingsManager);
    QNetworkSettingsUserAgent* agent = userAgent();
    if (agent)
        agent->setSsidAndPassphrase(name, passphrase);
    d->connectBySsid(name);
}

/*!
    Clears the connection state.
*/
void QNetworkSettingsManager::clearConnectionState()
{
    Q_D(QNetworkSettingsManager);
    d->clearConnectionState();
    QNetworkSettingsUserAgent* agent = userAgent();
    if (agent)
        agent->clearConnectionState();
}

/*!
    Attempts to connect using the next network interface in the model.
*/
void QNetworkSettingsManager::tryNextConnection()
{
    Q_D(QNetworkSettingsManager);
    d->tryNextConnection();
}

/*!
    Terminates the current connection to the specified \a service.
*/
void QNetworkSettingsManager::clearCurrentConnection(QNetworkSettingsService* service)
{
    Q_D(QNetworkSettingsManager);
    if (service->type() == QNetworkSettingsType::Wifi) {
        QNetworkSettingsService *currentService = d->currentWifiConnection();
        if (service == currentService) {
            d->setCurrentWifiConnection(nullptr);
            emit currentWifiConnectionChanged();
        }
    } else if (service->type() == QNetworkSettingsType::Wired) {
        QNetworkSettingsService *currentService = d->currentWiredConnection();
        if (service == currentService) {
            d->setCurrentWiredConnection(nullptr);
            emit currentWiredConnectionChanged();
        }
    }
}

/*!
    Creates a connection to the specified \a service.
*/
void QNetworkSettingsManager::setCurrentConnection(QNetworkSettingsService* service)
{
    Q_D(QNetworkSettingsManager);
    if (service->type() == QNetworkSettingsType::Wifi) {
        QNetworkSettingsService *currentService = d->currentWifiConnection();
        d->setCurrentWifiConnection(service);
        if (service != currentService)
            emit currentWifiConnectionChanged();
    } else if (service->type() == QNetworkSettingsType::Wired) {
        QNetworkSettingsService *currentService = d->currentWiredConnection();
        d->setCurrentWiredConnection(service);
        if (service != currentService)
            emit currentWiredConnectionChanged();
    }
}

/*!
    Returns the current Wifi connection.
*/
QNetworkSettingsService* QNetworkSettingsManager::currentWifiConnection()
{
    Q_D(QNetworkSettingsManager);
    return d->currentWifiConnection();
}

/*!
    Returns the current wired connection.
*/
QNetworkSettingsService* QNetworkSettingsManager::currentWiredConnection()
{
    Q_D(QNetworkSettingsManager);
    return d->currentWiredConnection();
}

/*!
    Returns the network interface instance \a instance of the type \a type.
*/
QNetworkSettingsInterface* QNetworkSettingsManager::interface(int type, int instance)
{
    Q_D(QNetworkSettingsManager);
    int matchingInstance = 0;

    foreach (QNetworkSettingsInterface* interface, d->m_interfaceModel.getModel()) {
        if (interface->type() == type) {
            if (matchingInstance == instance) {
                return interface;
            }
            matchingInstance++;
        }
    }
    return nullptr;
}

/*!
    Sets the user credentials for connecting to a network to \a agent.
*/
void QNetworkSettingsManager::setUserAgent(QNetworkSettingsUserAgent *agent)
{
    Q_D(QNetworkSettingsManager);
    d->setUserAgent(agent);
    connect(agent, &QNetworkSettingsUserAgent::requestNextConnection,
            this, &QNetworkSettingsManager::tryNextConnection);
}

/*!
    Returns the user credentials for connecting to a network.
*/
QNetworkSettingsUserAgent* QNetworkSettingsManager::userAgent()
{
    Q_D(QNetworkSettingsManager);
    return d->userAgent();
}

QT_END_NAMESPACE
