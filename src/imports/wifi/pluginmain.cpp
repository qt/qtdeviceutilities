#include "qwifimanager.h"

#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/qqml.h>

class QWifiPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")

public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("Qt.labs.wifi"));

        qmlRegisterType<QWifiManager>(uri, 0, 1, "QWifiManager");
    }
};

#include "pluginmain.moc"


