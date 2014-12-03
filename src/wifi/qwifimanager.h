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
#ifndef QWIFIMANAGER_H
#define QWIFIMANAGER_H

#include "qwificonfiguration.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QByteArray>
#include <QtCore/QLoggingCategory>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(B2QT_WIFI)

class QWifiManagerPrivate;
class QAbstractListModel;
class QWifiController;
class QWifiNetworkListModel;

class Q_DECL_EXPORT QWifiManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(NetworkState)
    Q_ENUMS(BackendState)
    Q_PROPERTY(NetworkState networkState READ networkState NOTIFY networkStateChanged)
    Q_PROPERTY(BackendState backendState READ backendState NOTIFY backendStateChanged)
    Q_PROPERTY(bool scanning READ isScanning WRITE setScanning NOTIFY scanningChanged)
    Q_PROPERTY(QString currentSSID READ currentSSID NOTIFY currentSSIDChanged)
    Q_PROPERTY(QString lastError READ lastError NOTIFY lastErrorChanged)
    Q_PROPERTY(QAbstractListModel *networks READ networks CONSTANT)
public:
    enum NetworkState {
        Disconnected,
        Authenticating,
        HandshakeFailed,
        ObtainingIPAddress,
        DhcpRequestFailed,
        Connected
    };

    enum BackendState {
        Initializing,
        Running,
        Terminating,
        NotRunning
    };

    enum Roles {
        SSID           = Qt::UserRole + 1,
        BSSID          = Qt::UserRole + 2,
        SignalStrength = Qt::UserRole + 3,
        WPASupported   = Qt::UserRole + 4,
        WPA2Supported  = Qt::UserRole + 5,
        WEPSupported   = Qt::UserRole + 6,
        WPSSupported   = Qt::UserRole + 7
    };

    static QWifiManager *instance();
    virtual ~QWifiManager();

    QAbstractListModel *networks() const;
    QString currentSSID() const;
    bool isScanning() const;
    void setScanning(bool scanning);
    NetworkState networkState() const;
    BackendState backendState() const;
    QString lastError() const;

public slots:
    void start();
    void stop();
    bool connect(QWifiConfiguration *config);
    void disconnect();

signals:
    void scanningChanged(bool scanning);
    void networkStateChanged(NetworkState networkState);
    void backendStateChanged(BackendState backendState);
    void currentSSIDChanged(const QString &currentSSID);
    void lastErrorChanged(const QString &error);

protected:
    bool event(QEvent *event);

private slots:
    void handleBackendStateChanged(BackendState backendState);
    void handleDhcpRequestFinished(const QString &status);

private:
    QWifiManager();
    static QWifiManager* m_instance;
    friend class QWifiController;
    Q_DISABLE_COPY(QWifiManager)
    Q_DECLARE_PRIVATE(QWifiManager)
    QWifiManagerPrivate *const d_ptr;
};

QT_END_NAMESPACE

#endif // QWIFIMANAGER_H
