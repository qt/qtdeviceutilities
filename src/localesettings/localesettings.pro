load(qt_build_config)

TARGET = QtLocaleSettings
VERSION = 1.0

QT = core dbus

MODULE = localesettings
load(qt_module)

DBUS_INTERFACES = locale.xml

HEADERS = \
    systemlocale.h \
    systemlocale_p.h \
    localemodel.h \
    localefiltermodel.h
SOURCES += systemlocale.cpp \
    localemodel.cpp \
    localefiltermodel.cpp

