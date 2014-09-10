/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
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
#include <b2qtdevice.h>
#include <QtQml>

static QObject *module_api_factory(QQmlEngine *engine, QJSEngine *scriptEngine)
{
   Q_UNUSED(engine)
   Q_UNUSED(scriptEngine)
   B2QtDevice *api = new B2QtDevice();

   return api;
}

class B2QtUtilsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    B2QtUtilsPlugin()
    {
    }

    void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == "B2Qt.Utils");
        qmlRegisterSingletonType<B2QtDevice>(uri, 1, 0, "B2QtDevice", module_api_factory);
    }
};

#include "plugin.moc"
