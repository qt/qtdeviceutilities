TEMPLATE = lib
CONFIG += plugin
QT += qml

uri = com.theqtcompany.settings.general

DESTDIR = imports/GeneralSettings
TARGET  = qmlgeneralsettingsplugin

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

