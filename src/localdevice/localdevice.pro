load(qt_build_config)

TARGET = QtLocalDevice
VERSION = 1.0
CONFIG += dll warn_on

QT = core

MODULE = localdevice
load(qt_module)

HEADERS += \
    $$PWD/qlocaldevice.h

SOURCES += \
    $$PWD/qlocaldevice.cpp
