#include <QtDroidUtils/qdroidutils.h>
#include <QtQml>

static QObject *module_api_factory(QQmlEngine *engine, QJSEngine *scriptEngine)
{
   Q_UNUSED(engine)
   Q_UNUSED(scriptEngine)
   QDroidUtils *api = new QDroidUtils();

   return api;
}

class QDroidUtilsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    QDroidUtilsPlugin()
    {
    }

    void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == "QtDroid.Utils");
        qmlRegisterSingletonType<QDroidUtils>(uri, 1, 0, "DroidUtils", module_api_factory);
    }
};

#include "plugin.moc"
