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
#include <B2QtWifi/QWifiManager>
#include <B2QtWifi/QWifiDevice>
#include <B2QtWifi/QWifiConfiguration>

#include <QtQml>

QT_BEGIN_NAMESPACE

static QObject *globalWifiDevice(QQmlEngine *, QJSEngine *)
{
    return new QWifiDevice;
}

static QObject *globalWifiManager(QQmlEngine *, QJSEngine *)
{
    return QWifiManager::instance();
}

class QWifiPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")

public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("B2Qt.Wifi"));

        qmlRegisterType<QAbstractListModel>();
        qmlRegisterSingletonType<QWifiManager>(uri, 1, 0, "WifiManager", globalWifiManager);
        qmlRegisterSingletonType<QWifiDevice>(uri, 1, 0, "WifiDevice", globalWifiDevice);
        qmlRegisterType<QWifiConfiguration>(uri, 1, 0, "WifiConfiguration");
    }
};

QT_END_NAMESPACE

#include "pluginmain.moc"
