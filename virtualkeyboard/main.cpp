#include <QQuickView>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("exampleplatforminputcontext"));

    QGuiApplication app(argc, argv);
    QQuickView view(QString("virtualkeyboard.qml"));
    view.show();

    return app.exec();
}
