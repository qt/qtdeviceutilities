TEMPLATE = lib
CONFIG += plugin
QT += qml timedatesettings

uri = com.theqtcompany.settings.timedate

DESTDIR = imports/TimeDate
TARGET  = qmltimedatesettingsplugin

SOURCES += plugin.cpp

pluginfiles.files += qmldir

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

target.path = $$installPath
pluginfiles.path += $$installPath
INSTALLS += target pluginfiles

