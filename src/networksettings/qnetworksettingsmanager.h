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

#include <QObject>

QT_BEGIN_NAMESPACE

class QNetworkSettingsService;
class QNetworkSettingsInterface;
class QNetworkSettingsUserAgent;
class QNetworkSettingsServiceFilter;
class QNetworkSettingsInterfaceModel;
class QNetworkSettingsManagerPrivate;
class Q_DECL_EXPORT QNetworkSettingsManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QNetworkSettingsServiceFilter* services READ services NOTIFY servicesChanged)
    Q_PROPERTY(QNetworkSettingsInterfaceModel* interfaces READ interfaces NOTIFY interfacesChanged)
    Q_PROPERTY(QNetworkSettingsUserAgent* userAgent READ userAgent CONSTANT)
    Q_PROPERTY(QNetworkSettingsService* currentWifiConnection READ currentWifiConnection NOTIFY currentWifiConnectionChanged)
    Q_PROPERTY(QNetworkSettingsService* currentWiredConnection READ currentWiredConnection NOTIFY currentWiredConnectionChanged)
    Q_MOC_INCLUDE("qnetworksettingsservicemodel.h")
    Q_MOC_INCLUDE("qnetworksettingsuseragent.h")

public:
    explicit QNetworkSettingsManager(QObject* parent = nullptr);
    QNetworkSettingsServiceFilter* services();
    QNetworkSettingsInterfaceModel* interfaces();
    void setUserAgent(QNetworkSettingsUserAgent *agent);
    QNetworkSettingsUserAgent* userAgent();

    Q_INVOKABLE QNetworkSettingsService* service(const QString& name, int type);
    Q_INVOKABLE void connectBySsid(const QString& name, const QString &passphrase);
    void clearConnectionState();
    void tryNextConnection();
    void clearCurrentConnection(QNetworkSettingsService* service);
    void setCurrentConnection(QNetworkSettingsService* service);
    QNetworkSettingsService* currentWifiConnection();
    QNetworkSettingsService* currentWiredConnection();
    Q_INVOKABLE QNetworkSettingsInterface* interface(int type, int instance);

Q_SIGNALS:
    void servicesChanged();
    void interfacesChanged();
    void currentWifiConnectionChanged();
    void currentWiredConnectionChanged();

protected:
    QNetworkSettingsManagerPrivate *d_ptr;

private:
    Q_DISABLE_COPY(QNetworkSettingsManager)
    Q_DECLARE_PRIVATE(QNetworkSettingsManager)
};

QT_END_NAMESPACE

#endif // QNETWORKSETTINGSMANAGER_H
