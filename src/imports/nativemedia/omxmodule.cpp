#include "omxnode.h"
#include <QtQml/QQmlExtensionPlugin>

class QAndroidOmxModule : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")

public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtAndroidOmx"));

        qmlRegisterType<OmxItem>(uri, 1, 0, "OmxItem");
    }
};

#include "omxmodule.moc"

