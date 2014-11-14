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
#include "qwifimanager.h"
#include "qwifiinterface.h"

#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/qqml.h>

static QObject *global_object_wifi(QQmlEngine *, QJSEngine *)
{
    return new QWifiInterface;
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
        qmlRegisterType<QWifiNetworkListModel>();
        qmlRegisterSingletonType<QWifiInterface>(uri, 0, 1, "Interface", global_object_wifi);
    }
};

#include "pluginmain.moc"
