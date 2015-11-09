TEMPLATE = lib
CONFIG += plugin
QT += qml

uri = com.theqtcompany.settings.wifi

DESTDIR = imports/Wifi
TARGET  = qmlwifiplugin

SOURCES += plugin.cpp

pluginfiles.files += \
    qmldir \

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

target.path = $$installPath
pluginfiles.path += $$installPath
INSTALLS += target pluginfiles


RESOURCES += \
    icons.qrc \
    qml.qrc

HEADERS +=
