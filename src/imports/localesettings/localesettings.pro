QT += qml localesettings

CONFIG += no_cxx_module

TARGET  = localesettingsplugin
TARGETPATH = QtDeviceUtilities/LocaleSettings

SOURCES += plugin.cpp

load(qml_plugin)
