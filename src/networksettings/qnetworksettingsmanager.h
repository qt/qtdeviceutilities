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
#ifndef QNETWORKSETTINGSMANAGER_H
#define QNETWORKSETTINGSMANAGER_H

#include "qnetworksettings.h"
#include <QObject>
#include <QStringListModel>

QT_FORWARD_DECLARE_CLASS(QNetworkSettingsManagerPrivate)
QT_FORWARD_DECLARE_CLASS(QNetworkSettingsService)
QT_FORWARD_DECLARE_CLASS(QNetworkSettingsUserAgent)

class Q_DECL_EXPORT QNetworkSettingsManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(StateTypes NetworkTypeTypes)
    Q_PROPERTY(QAbstractItemModel* services READ services NOTIFY servicesChanged)
    Q_PROPERTY(QAbstractItemModel* interfaces READ interfaces NOTIFY interfacesChanged)
public:
    explicit QNetworkSettingsManager(QObject* parent = 0);
    QAbstractItemModel* services();
    QAbstractItemModel* interfaces();
    void setUserAgent(QNetworkSettingsUserAgent *agent);

    Q_INVOKABLE QNetworkSettingsService* getService(const QString& name, const int type);

Q_SIGNALS:
    void servicesChanged();
    void interfacesChanged();

protected:
    QNetworkSettingsManagerPrivate *d_ptr;

private:
    Q_DISABLE_COPY(QNetworkSettingsManager)
    Q_DECLARE_PRIVATE(QNetworkSettingsManager)
};

#endif // QNETWORKSETTINGSMANAGER_H
