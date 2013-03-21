#include <QtGui>
#include <QtQml>
#include <QtQuick>

#include "qwifimanager.h"

#include <hardware_legacy/wifi.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<QWifiManager>("QtWifi", 0, 1, "QWifiManager");
    qmlRegisterType<QWifiNetwork>();
    qmlRegisterType<QWifiNetworkList>();

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl::fromLocalFile("main.qml"));
    view.show();

    return app.exec();
}
