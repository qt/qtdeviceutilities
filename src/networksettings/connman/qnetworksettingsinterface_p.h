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
#ifndef QNETWORKSETTINGSINTERFACEPRIVATE_H
#define QNETWORKSETTINGSINTERFACEPRIVATE_H

#include <QObject>
#include <QtDBus>
#include "qnetworksettings.h"
#include "qnetworksettingsinterface.h"

class NetConnmanTechnologyInterface;

class QNetworkSettingsInterfacePrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(QNetworkSettingsInterface)
public:
    explicit QNetworkSettingsInterfacePrivate(QNetworkSettingsInterface* parent);
    void initialize(const QString& path, const QVariantMap& properties);
    void setPowered(const bool power);
    void setState(QNetworkSettingsState::States aState);
    void scan();

public slots:
    void updateProperty(const QString &name, const QDBusVariant &value);
protected:
    void updateProperty(const QString &name, const QVariant &value);

protected:
    NetConnmanTechnologyInterface *m_technology;
    QString m_name;
    QNetworkSettingsType m_type;
    QNetworkSettingsState m_state;
    bool m_powered;
    QNetworkSettingsInterface *q_ptr;
};


class ConnmanSettingsInterface : public QNetworkSettingsInterface
{
    Q_OBJECT
public:
    ConnmanSettingsInterface(const QString& path, const QVariantMap& properties, QObject *parent=0)
        :QNetworkSettingsInterface(parent)
    {
        if (d_ptr)
            d_ptr->initialize(path, properties);
    }

    void setState(QNetworkSettingsState::States aState) {
        Q_D(QNetworkSettingsInterface);
        d->setState(aState);
    }

    virtual ~ConnmanSettingsInterface() {

    }
};
#endif // QNETWORKSETTINGSINTERFACEPRIVATE_H
