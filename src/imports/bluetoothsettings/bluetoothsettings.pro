QT += qml bluetooth bluetoothsettings

CONFIG += no_cxx_module

TARGET  = btsettingsplugin
TARGETPATH = QtDeviceUtilities/BluetoothSettings

SOURCES += plugin.cpp

load(qml_plugin)
