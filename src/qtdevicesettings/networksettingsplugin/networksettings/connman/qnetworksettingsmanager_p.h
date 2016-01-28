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
#ifndef QNETWORKSETTINGSMANAGERPRIVATE_H
#define QNETWORKSETTINGSMANAGERPRIVATE_H

#include <QObject>
#include <QtDBus>
#include "connmancommon.h"
#include "qnetworksettingsmanager.h"
#include "qnetworksettingsinterfacemodel.h"
#include "qnetworksettingsservicemodel.h"

class NetConnmanManagerInterface;

class QNetworkSettingsManagerPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(QNetworkSettingsManager)
public:
    explicit QNetworkSettingsManagerPrivate(QNetworkSettingsManager *parent);
    QNetworkSettingsManager *q_ptr;

public slots:
    void getServicesFinished(QDBusPendingCallWatcher *watcher);
    void getTechnologiesFinished(QDBusPendingCallWatcher *watcher);
    void requestInput(const QString& service, const QString& type);
    void servicesChanged(ConnmanMapList changed, const QList<QDBusObjectPath> &removed);

protected:
    QNetworkSettingsInterfaceModel m_interfaceModel;
    QNetworkSettingsServiceModel m_serviceModel;
    QNetworkSettingsServiceFilter m_serviceFilter;
private:
    NetConnmanManagerInterface *m_manager;
};

#endif // QNETWORKSETTINGSMANAGERPRIVATE_H
