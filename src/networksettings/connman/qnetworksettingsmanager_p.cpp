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
#include "qnetworksettingsmanager_p.h"
#include "connman_manager_interface.cpp"
#include "moc_connman_manager_interface.cpp"
#include "qnetworksettingsinterface.h"
#include "qnetworksettingsinterface_p.h"
#include "qnetworksettingsservicemodel.h"
#include "qnetworksettingsuseragent.h"

QNetworkSettingsManagerPrivate::QNetworkSettingsManagerPrivate(QNetworkSettingsManager *parent)
    :QObject(parent)
    ,q_ptr(parent)
    , m_interfaceModel(Q_NULLPTR)
    , m_serviceModel(Q_NULLPTR)
    , m_serviceFilter(Q_NULLPTR)
    , m_manager(Q_NULLPTR)
    , m_agent(Q_NULLPTR)
{
    qDBusRegisterMetaType<ConnmanMapStruct>();
    qDBusRegisterMetaType<ConnmanMapStructList>();

    QNetworkSettingsUserAgent* userAgent = new QNetworkSettingsUserAgent(this);
    this->setUserAgent(userAgent);

    m_serviceModel = new QNetworkSettingsServiceModel(this);
    m_serviceFilter = new QNetworkSettingsServiceFilter(this);
    m_serviceFilter->setSourceModel(m_serviceModel);
    m_manager = new NetConnmanManagerInterface(QStringLiteral("net.connman"), QStringLiteral("/"),
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

        m_manager->RegisterAgent(QDBusObjectPath(AgentPath));
    }
    else {
        delete m_manager;
        m_manager = NULL;
    }
}

void QNetworkSettingsManagerPrivate::requestInput(const QString& service, const QString& type)
{
    Q_UNUSED(type);
    foreach (QNetworkSettingsService* servicePtr, m_serviceModel->getModel()) {
        if (servicePtr->id() == service)
            emit servicePtr->showCrendentialInput();
    }
    m_agent->showUserCredentialsInput();
}

void QNetworkSettingsManagerPrivate::getServicesFinished(QDBusPendingCallWatcher *watcher)
{
    Q_Q(QNetworkSettingsManager);
    QDBusPendingReply<ConnmanMapStructList> reply = *watcher;
    watcher->deleteLater();

    if (reply.isError())
        return;

    foreach (const ConnmanMapStruct &object, reply.value()) {
        const QString servicePath = object.objectPath.path();
        handleNewService(servicePath);
    }
    emit q->servicesChanged();
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
    foreach (QDBusObjectPath path, removed) {
        m_serviceModel->removeService(path.path());
    }

    QStringList newServices;
    foreach (ConnmanMapStruct map, changed) {
        bool found = false;
        foreach (QNetworkSettingsService* service, m_serviceModel->getModel()) {
            if (service->id() == map.objectPath.path()) {
                found =true;
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

    if (service->name().length() > 0 && service->type() != QNetworkSettingsType::Unknown) {
        m_serviceModel->append(service);
        emit q->servicesChanged();
        if (service->type() == QNetworkSettingsType::Wired) {
            m_serviceFilter->setWiredNetworksAvailable(true);
        }
    }
    else {
        //Service name or type not set, wait for update
        connect(service, &QNetworkSettingsService::nameChanged, this, &QNetworkSettingsManagerPrivate::serviceReady);
        connect(service, &QNetworkSettingsService::typeChanged, this, &QNetworkSettingsManagerPrivate::serviceReady);
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
    if (service->name().length() > 0 && service->type() != QNetworkSettingsType::Unknown) {
        service->disconnect(this);
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
