load(qt_build_config)

TARGET = QtDisplaySettings
VERSION = 1.0

QT = core

MODULE = displaysettings
load(qt_module)

SOURCES += displaysettings.cpp \
    displaysettings_p.cpp

HEADERS += displaysettings.h \
    displaysettings_p.h

