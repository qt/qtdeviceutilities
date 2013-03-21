#include <QtQuick/QQuickView>
#include <QtGui/QGuiApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("exampleplatforminputcontext"));

    QGuiApplication app(argc, argv);
    QQuickView view(QString("virtualkeyboard.qml"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();

    return app.exec();
}
