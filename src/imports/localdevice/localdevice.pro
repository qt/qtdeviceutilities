TEMPLATE = lib
CONFIG += plugin
TARGET  = localdeviceplugin
uri = com.theqtcompany.localdevice

QT += qml localdevice

pluginfiles.files += \
    qmldir \

SOURCES += plugin.cpp

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

target.path = $$installPath
pluginfiles.path += $$installPath
INSTALLS += target pluginfiles
