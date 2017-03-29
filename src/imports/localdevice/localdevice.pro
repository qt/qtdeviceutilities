QT += qml localdevice

CONFIG += no_cxx_module

TARGET  = localdeviceplugin
TARGETPATH = QtDeviceUtilities/LocalDeviceSettings

SOURCES += plugin.cpp

load(qml_plugin)
