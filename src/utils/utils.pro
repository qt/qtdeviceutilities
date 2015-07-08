load(qt_build_config)

TARGET = B2QtUtils
VERSION = 1.0
CONFIG += dll warn_on

QT = core network

MODULE = b2qtutils
load(qt_module)

HEADERS += \
    $$PWD/b2qtdevice.h

SOURCES += \
    $$PWD/b2qtdevice.cpp
