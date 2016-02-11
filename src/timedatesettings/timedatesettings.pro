load(qt_build_config)

TARGET = QtTimeDateSettings
VERSION = 1.0
CONFIG += dll warn_on

QT = core dbus

MODULE = timedatesettings
load(qt_module)

DBUS_INTERFACES = timedated.xml

SOURCES += systemtime.cpp \
    timezonemodel.cpp \
    timezonefiltermodel.cpp

HEADERS += \
    systemtime.h \
    systemtime_p.h \
    timezonemodel.h \
    timezonefiltermodel.h

