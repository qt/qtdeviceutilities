#-------------------------------------------------
#
# Project created by QtCreator 2015-11-03T13:52:45
#
#-------------------------------------------------

QT       += core dbus

INCLUDEPATH += $${PWD}
INCLUDEPATH += $${PWD}/bluez

DBUS_INTERFACES = \
    $${PWD}/objectmanager.xml \
    $${PWD}/device1.xml \

HEADERS += \
    $$PWD/bluetoothdevice_p.h \
    $$PWD/dbusdatatypes.h

SOURCES += \
    $$PWD/bluetoothdevice_p.cpp
