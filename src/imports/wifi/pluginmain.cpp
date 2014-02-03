/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://qt.digia.com/
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://qt.digia.com/
**
****************************************************************************/
#include "qwifimanager.h"

#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/qqml.h>

#include <QtCore/QDir>
#include <QtCore/QByteArray>

class QWifiGlobal : public QObject
{
    Q_OBJECT
public:
    explicit QWifiGlobal(QObject *parent = 0)
        : QObject(parent) {}
    ~QWifiGlobal() {}

    Q_INVOKABLE bool wifiSupported() const
    {
        char interface[PROPERTY_VALUE_MAX];
        property_get("wifi.interface", interface, NULL);
        // standard linux kernel path
        QByteArray path;
        path.append("/sys/class/net/").append(interface);
        bool interfaceFound = QDir().exists(path.constData());
        if (!interfaceFound)
            qWarning() << "QWifiGlobal: could not find wifi interface in " << path;
        return interfaceFound;
    }
};

static QObject *global_object_wifi(QQmlEngine *, QJSEngine *)
{
    return new QWifiGlobal;
}

class QWifiPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")

public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("Qt.labs.wifi"));

        qmlRegisterType<QWifiManager>(uri, 0, 1, "WifiManager");
        qmlRegisterType<QWifiNetworkList>();
        qmlRegisterSingletonType<QWifiGlobal>(uri, 0, 1, "Interface", global_object_wifi);
    }
};

#include "pluginmain.moc"
