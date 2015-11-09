TEMPLATE = lib
CONFIG += plugin
QT += qml dbus

uri = com.theqtcompany.settings.timedate

DBUS_INTERFACES = timedated.xml

DESTDIR = imports/TimeDate
TARGET  = qmltimedatesettingsplugin

SOURCES += plugin.cpp \
    systemtime.cpp \
    timezonemodel.cpp \
    timezonefiltermodel.cpp

pluginfiles.files += \
    qmldir


pluginfiles.files += \
    AnalogClock.qml \
    TimeDate.qml \
    TimezonesView.qml \

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

target.path = $$installPath
pluginfiles.path += $$installPath
INSTALLS += target pluginfiles


RESOURCES += \
    icons.qrc

HEADERS += \
    systemtime.h \
    systemtime_p.h \
    timezonemodel.h \
    timezonefiltermodel.h


