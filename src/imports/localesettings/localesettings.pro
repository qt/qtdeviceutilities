TEMPLATE = lib
CONFIG += plugin
QT += qml localesettings

uri = QtDeviceUtilities.LocaleSettings

TARGET  = localesettingsplugin

SOURCES += plugin.cpp

pluginfiles.files += \
    qmldir \

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

target.path = $$installPath
pluginfiles.path += $$installPath
INSTALLS += target pluginfiles
