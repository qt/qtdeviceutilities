QT  += core dbus

INCLUDEPATH += $${PWD}
INCLUDEPATH += $${PWD}/bluez
INCLUDEPATH += $${PWD}/../../networksettings/connman

DBUS_INTERFACES = \
    $${PWD}/objectmanager.xml \
    $${PWD}/device1.xml \
    $${PWD}/../../networksettings/connman/connman_technology.xml

HEADERS += \
    $$PWD/bluetoothdevice_p.h \
    $$PWD/datatypes.h \

SOURCES += \
    $$PWD/bluetoothdevice_p.cpp
