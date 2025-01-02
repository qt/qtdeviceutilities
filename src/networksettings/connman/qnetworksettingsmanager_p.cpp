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
#include "qnetworksettingsservice.h"
#include "qnetworksettingsmanager_p.h"
#include "qnetworksettingsinterface_p.h"
#include "qnetworksettingsservicemodel.h"
#include "qnetworksettingsuseragent.h"
#include "moc_connman_manager_interface.cpp"

QT_BEGIN_NAMESPACE

const QString ConnManServiceName(QStringLiteral("net.connman"));

QNetworkSettingsManagerPrivate::QNetworkSettingsManagerPrivate(QNetworkSettingsManager *parent)
    :QObject(parent)
    ,q_ptr(parent)
    , m_interfaceModel(nullptr)
    , m_serviceModel(nullptr)
    , m_serviceFilter(nullptr)
    , m_manager(nullptr)
    , m_agent(nullptr)
    , m_serviceWatcher(nullptr)
    , m_currentWifiConnection(nullptr)
    , m_currentWiredConnection(nullptr)
    , m_initialized(false)
{
    qRegisterMetaType<ConnmanMapStruct>("ConnmanMapStruct");
    qRegisterMetaType<ConnmanMapStructList>("ConnmanMapStructList");
    qDBusRegisterMetaType<ConnmanMapStruct>();
    qDBusRegisterMetaType<ConnmanMapStructList>();

    QNetworkSettingsUserAgent* userAgent = new QNetworkSettingsUserAgent(this);
    this->setUserAgent(userAgent);

    m_serviceModel = new QNetworkSettingsServiceModel(this);
    m_serviceFilter = new QNetworkSettingsServiceFilter(this);
    m_serviceFilter->setSourceModel(m_serviceModel);

    QDBusConnectionInterface* bus = QDBusConnection::systemBus().interface();
    if (bus->isServiceRegistered(ConnManServiceName)) {
        if (!initialize())
            qWarning("Failed to initialize connman connection");
    } else {
        m_serviceWatcher = new QDBusServiceWatcher(this);
        m_serviceWatcher->setConnection(QDBusConnection::systemBus());
        m_serviceWatcher->setWatchedServices(QStringList({ConnManServiceName}));
        connect(m_serviceWatcher, &QDBusServiceWatcher::serviceRegistered, this, &QNetworkSettingsManagerPrivate::onConnmanServiceRegistered);
    }
}

bool QNetworkSettingsManagerPrivate::initialize()
{
    if (m_initialized && m_manager)
        return m_initialized;

    m_manager = new NetConnmanManagerInterface(ConnManServiceName, QStringLiteral("/"),
            QDBusConnection::systemBus(), this);

    if (m_manager->isValid()) {
        //List technologies
        QDBusPendingReply<ConnmanMapStructList> reply = m_manager->GetTechnologies();
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
        connect(watcher, &QDBusPendingCallWatcher::finished,
                this, &QNetworkSettingsManagerPrivate::getTechnologiesFinished);

        reply = m_manager->GetServices();
        watcher = new QDBusPendingCallWatcher(reply, this);
        connect(watcher, &QDBusPendingCallWatcher::finished,
               this, &QNetworkSettingsManagerPrivate::getServicesFinished);

        connect(m_manager, &NetConnmanManagerInterface::ServicesChanged, this, &QNetworkSettingsManagerPrivate::onServicesChanged);
        connect(m_manager, &NetConnmanManagerInterface::TechnologyAdded, this, &QNetworkSettingsManagerPrivate::onTechnologyAdded);
        connect(m_manager, &NetConnmanManagerInterface::TechnologyRemoved, this, &QNetworkSettingsManagerPrivate::onTechnologyRemoved);

        m_manager->RegisterAgent(QDBusObjectPath(PropertyAgentPath));
        m_initialized = true;
    } else {
        delete m_manager;
        m_manager = nullptr;
        m_initialized = false;
    }
    return m_initialized;
}

void QNetworkSettingsManagerPrivate::requestInput(const QString& service, const QString& type)
{
    Q_UNUSED(service)
    Q_UNUSED(type)
    emit m_agent->showUserCredentialsInput();
}

void QNetworkSettingsManagerPrivate::connectBySsid(const QString &name)
{
    m_unnamedServicesForSsidConnection = m_unnamedServices;
    tryNextConnection();
    m_currentSsid = name;
}

void QNetworkSettingsManagerPrivate::clearConnectionState()
{
    m_unnamedServicesForSsidConnection.clear();
    m_currentSsid.clear();
}

void QNetworkSettingsManagerPrivate::tryNextConnection()
{
    Q_Q(QNetworkSettingsManager);
    QNetworkSettingsService* service = nullptr;
    if (!m_currentSsid.isEmpty()) {
        service = m_serviceModel->getByName(m_currentSsid);
        m_currentSsid.clear();
    }
    if (!service) {
        if (!m_unnamedServicesForSsidConnection.isEmpty()) {
            service = m_unnamedServicesForSsidConnection.take(m_unnamedServicesForSsidConnection.firstKey());
        } else {
            q->clearConnectionState();
        }
    }
    if (service) {
        service->doConnectService();
    }
}

void QNetworkSettingsManagerPrivate::setCurrentWifiConnection(QNetworkSettingsService *connection)
{
    m_currentWifiConnection = connection;
}

QNetworkSettingsService *QNetworkSettingsManagerPrivate::currentWifiConnection() const
{
    return m_currentWifiConnection.data();
}

void QNetworkSettingsManagerPrivate::setCurrentWiredConnection(QNetworkSettingsService *connection)
{
    m_currentWiredConnection = connection;
}

QNetworkSettingsService *QNetworkSettingsManagerPrivate::currentWiredConnection() const
{
    return m_currentWiredConnection.data();
}

void QNetworkSettingsManagerPrivate::onConnmanServiceRegistered(const QString &serviceName)
{
    if (serviceName == ConnManServiceName) {
        if (!initialize())
            qWarning("Failed to initialize connman connection");
    }
}

void QNetworkSettingsManagerPrivate::onTechnologyAdded(const QDBusObjectPath &technology, const QVariantMap &properties)
{
    Q_Q(QNetworkSettingsManager);

    foreach (QNetworkSettingsInterface* item, m_interfaceModel.getModel()) {
        ConnmanSettingsInterface* interface = qobject_cast<ConnmanSettingsInterface*>(item);
        if (interface->path() == technology.path()) {
            return; // we already know the interface/technology
        }
    }

    ConnmanSettingsInterface *interface = new ConnmanSettingsInterface(technology.path(), properties, this);
    interface->scanServices();

    if (interface->type() == QNetworkSettingsType::Wired) {
        m_interfaceModel.insert(0, interface);
    }
    else if (interface->type() == QNetworkSettingsType::Wifi) {
        m_interfaceModel.append(interface);
    }

    emit q->interfacesChanged();
}

void QNetworkSettingsManagerPrivate::onTechnologyRemoved(const QDBusObjectPath &technology)
{
    Q_Q(QNetworkSettingsManager);

    foreach (QNetworkSettingsInterface* item, m_interfaceModel.getModel()) {
        ConnmanSettingsInterface* tech = qobject_cast<ConnmanSettingsInterface*>(item);
        if (tech->path() == technology.path()) {
            m_interfaceModel.removeInterface(tech->name());
            emit q->interfacesChanged();
        }
    }
}

void QNetworkSettingsManagerPrivate::getServicesFinished(QDBusPendingCallWatcher *watcher)
{
    QDBusPendingReply<ConnmanMapStructList> reply = *watcher;
    watcher->deleteLater();

    if (reply.isError())
        return;

    foreach (const ConnmanMapStruct &object, reply.value()) {
        const QString servicePath = object.objectPath.path();
        handleNewService(servicePath);
    }
}

void QNetworkSettingsManagerPrivate::getTechnologiesFinished(QDBusPendingCallWatcher *watcher)
{
    Q_Q(QNetworkSettingsManager);

    QDBusPendingReply<ConnmanMapStructList> reply = *watcher;
    watcher->deleteLater();
    if (reply.isError())
        return;

    foreach (const ConnmanMapStruct &object, reply.value()) {
        ConnmanSettingsInterface *item = new ConnmanSettingsInterface(object.objectPath.path(), object.propertyMap, this);
        item->scanServices();

        if (item->type() == QNetworkSettingsType::Wired) {
            m_interfaceModel.insert(0, item);
        }
        else if (item->type() == QNetworkSettingsType::Wifi) {
            m_interfaceModel.append(item);
        }
        emit q->interfacesChanged();
    }
}

void QNetworkSettingsManagerPrivate::onServicesChanged(ConnmanMapStructList changed, const QList<QDBusObjectPath> &removed)
{
    Q_Q(QNetworkSettingsManager);
    foreach (const QDBusObjectPath &dpath, removed) {
        QString path = dpath.path();

        if (m_serviceModel->removeService(path))
            emit q->servicesChanged();

        if (auto service = m_unnamedServices.take(path))
            if (!service->placeholderState()) {
                disconnect(service, &QNetworkSettingsService::nameChanged,
                           this, &QNetworkSettingsManagerPrivate::serviceReady);
                disconnect(service, &QNetworkSettingsService::typeChanged,
                           this, &QNetworkSettingsManagerPrivate::serviceReady);
                service->deleteLater();
            }
        if (auto service = m_unknownServices.take(path))
            if (!service->placeholderState()) {
                disconnect(service, &QNetworkSettingsService::nameChanged,
                           this, &QNetworkSettingsManagerPrivate::serviceReady);
                disconnect(service, &QNetworkSettingsService::typeChanged,
                           this, &QNetworkSettingsManagerPrivate::serviceReady);
                service->deleteLater();
            }
        m_unnamedServicesForSsidConnection.remove(path); // do not delete here
    }

    QStringList newServices;
    foreach (const ConnmanMapStruct &map, changed) {
        QString path = map.objectPath.path();

        if (m_unknownServices.contains(path) || m_unnamedServices.contains(path))
            continue;

        bool found = false;
        foreach (QNetworkSettingsService* service, m_serviceModel->getModel()) {
            if (service->id() == path && service->placeholderState() == false) {
                found = true;
                break;
            }
        }
        if (!found)
            newServices.append(map.objectPath.path());
    }

    foreach (QString newService, newServices) {
        handleNewService(newService);
    }

}

void QNetworkSettingsManagerPrivate::handleNewService(const QString &servicePath)
{
    Q_Q(QNetworkSettingsManager);

    QNetworkSettingsService *service = new QNetworkSettingsService(servicePath, this);

    connect(service, &QNetworkSettingsService::connectionStateCleared,
            q, &QNetworkSettingsManager::clearConnectionState);

    connect(service, &QNetworkSettingsService::serviceConnected,
            q, &QNetworkSettingsManager::setCurrentConnection);
    connect(service, &QNetworkSettingsService::serviceDisconnected,
            q, &QNetworkSettingsManager::clearCurrentConnection);

    if (service->name().length() > 0 && service->type() != QNetworkSettingsType::Unknown) {
        m_serviceModel->append(service);
        emit q->servicesChanged();
        if (service->type() == QNetworkSettingsType::Wired) {
            m_serviceFilter->setWiredNetworksAvailable(true);
        }
    }
    else {
        bool isUnnamedWifi = false;

        //Service name or type not set, wait for update
        if (service->name().isEmpty()) {
            connect(service, &QNetworkSettingsService::nameChanged, this, &QNetworkSettingsManagerPrivate::serviceReady);
            isUnnamedWifi = (service->type() == QNetworkSettingsType::Wifi);
        }
        if (service->type() == QNetworkSettingsType::Unknown)
            connect(service, &QNetworkSettingsService::typeChanged, this, &QNetworkSettingsManagerPrivate::serviceReady);

        if (isUnnamedWifi)
            m_unnamedServices.insert(service->id(), service);
        else
            m_unknownServices.insert(service->id(), service);
    }
}

void QNetworkSettingsManagerPrivate::setUserAgent(QNetworkSettingsUserAgent *agent)
{
    m_agent = agent;
}

void QNetworkSettingsManagerPrivate::serviceReady()
{
    Q_Q(QNetworkSettingsManager);

    QNetworkSettingsService* service = qobject_cast<QNetworkSettingsService*>(sender());

    // the type changed from Unknown to Wifi
    if ((service->type() == QNetworkSettingsType::Wifi)
            && m_unknownServices.contains(service->id())) {
        m_unknownServices.remove(service->id());
        m_unnamedServices.insert(service->id(), service);
    }

    // we have a name and a length now
    if (service->name().length() > 0 && service->type() != QNetworkSettingsType::Unknown) {
        service->disconnect(this);
        m_unnamedServices.remove(service->id());
        m_serviceModel->append(service);
        emit q->servicesChanged();
        if (service->type() == QNetworkSettingsType::Wired) {
            m_serviceFilter->setWiredNetworksAvailable(true);
        }

        //Update the interface state accordingly
        foreach (QNetworkSettingsInterface* item, m_interfaceModel.getModel()) {
            ConnmanSettingsInterface* technology = qobject_cast<ConnmanSettingsInterface*>(item);
            if (technology->name() == service->name() && technology->type() == service->type()) {
                technology->setState(technology->state());
            }
        }
    }
}

QT_END_NAMESPACE
