load(qt_build_config)

TARGET = QtBluetoothSettings
VERSION = 1.0
CONFIG += dll warn_on

QT += core bluetooth

MODULE = bluetoothsettings
load(qt_module)

include(bluez/bluez.pri)

HEADERS += \
    bluetoothdevice.h \
    discoverymodel.h

SOURCES += bluetoothdevice.cpp \
    discoverymodel.cpp
