/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
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

QNetworkSettingsManagerPrivate::QNetworkSettingsManagerPrivate(QNetworkSettingsManager *parent)
    :QObject(parent)
    ,q_ptr(parent)
{
    m_serviceModel = new QNetworkSettingsServiceModel(this);
    m_serviceFilter = new QNetworkSettingsServiceFilter(this);
    m_serviceFilter->setSourceModel(m_serviceModel);
    m_manager = new NetConnmanManagerInterface(QStringLiteral("net.connman"), QStringLiteral("/"),
            QDBusConnection::systemBus(), this);

    if (m_manager->isValid()) {
        //List technologies
        QDBusPendingReply<ConnmanMapList> reply = m_manager->GetTechnologies();
        QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
        connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                this, SLOT(getTechnologiesFinished(QDBusPendingCallWatcher*)));

        reply = m_manager->GetServices();
        watcher = new QDBusPendingCallWatcher(reply, this);
        connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
               this, SLOT(getServicesFinished(QDBusPendingCallWatcher*)));

        connect(m_manager, &NetConnmanManagerInterface::ServicesChanged, this, &QNetworkSettingsManagerPrivate::servicesChanged);

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
}

void QNetworkSettingsManagerPrivate::getServicesFinished(QDBusPendingCallWatcher *watcher)
{
    Q_Q(QNetworkSettingsManager);
    QDBusPendingReply<ConnmanMapList> reply = *watcher;
    watcher->deleteLater();
    if (reply.isError())
        return;

    foreach (const ConnmanMap &object, reply.value()) {
        const QString servicePath = object.objectPath.path();
        QNetworkSettingsService *service = new QNetworkSettingsService(servicePath, this);
        m_serviceModel->append(service);

        //Update status property
        QString n = qdbus_cast<QString>(object.propertyMap[PropertyName]);
        QString t = qdbus_cast<QString>(object.propertyMap[PropertyType]);
        QString s = qdbus_cast<QString>(object.propertyMap[PropertyState]);

        QNetworkSettingsType type;
        t >> type;
        QNetworkSettingsState state;
        s >> state;

        foreach (QNetworkSettingsInterface* item, m_interfaceModel.getModel()) {
            ConnmanSettingsInterface* technology = qobject_cast<ConnmanSettingsInterface*>(item);
            if (technology->name() == n && technology->type() == type.type()) {
                technology->setState(state.state());
            }
        }
    }
    emit q->servicesChanged();
}

void QNetworkSettingsManagerPrivate::getTechnologiesFinished(QDBusPendingCallWatcher *watcher)
{
    Q_Q(QNetworkSettingsManager);

    QDBusPendingReply<ConnmanMapList> reply = *watcher;
    watcher->deleteLater();
    if (reply.isError())
        return;

    foreach (const ConnmanMap &object, reply.value()) {
        ConnmanSettingsInterface *item = new ConnmanSettingsInterface(object.objectPath.path(), object.propertyMap, this);

        if (item->type() == QNetworkSettingsType::Wired) {
            m_interfaceModel.insert(0, item);
        }
        else if (item->type() == QNetworkSettingsType::Wifi) {
            m_interfaceModel.append(item);
        }
        emit q->interfacesChanged();
    }
}

void QNetworkSettingsManagerPrivate::servicesChanged(ConnmanMapList changed, const QList<QDBusObjectPath> &removed)
{
    foreach (QDBusObjectPath path, removed) {
        QList<QNetworkSettingsService*> serviceList = m_serviceModel->getModel();
        QMutableListIterator<QNetworkSettingsService*> i(serviceList);
        while (i.hasNext()) {
            if (i.next()->id() == path.path())
                i.remove();
        }
    }

    QStringList newServices;
    foreach (ConnmanMap map, changed) {
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
        QNetworkSettingsService *service = new QNetworkSettingsService(newService, this);
        m_serviceModel->append(service);
    }
}

void QNetworkSettingsManagerPrivate::setUserAgent(QNetworkSettingsUserAgent *agent)
{
    Q_UNUSED(agent);
}
