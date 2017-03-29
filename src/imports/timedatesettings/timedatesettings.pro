QT += qml timedatesettings

CONFIG += no_cxx_module

TARGET  = qmltimedatesettingsplugin
TARGETPATH = QtDeviceUtilities/TimeDateSettings

SOURCES += plugin.cpp

load(qml_plugin)
