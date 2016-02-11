QT       += core network dbus

INCLUDEPATH += $${PWD}/connman

DBUS_INTERFACES = \
    connman/connman_manager.xml \
    connman/connman_service.xml \
    connman/connman_technology.xml

SOURCES += \
    $$PWD/connman/qnetworksettingsinterface_p.cpp \
    $$PWD/connman/qnetworksettingsservice_p.cpp \
    $$PWD/connman/qnetworksettingsuseragent_p.cpp \
    $$PWD/connman/qnetworksettingsmanager_p.cpp \
    $$PWD/connman/connmancommon.cpp

HEADERS  += \
    $$PWD/connman/qnetworksettingsinterface_p.h \
    $$PWD/connman/qnetworksettingsservice_p.h \
    $$PWD/connman/qnetworksettingsuseragent_p.h \
    $$PWD/connman/qnetworksettingsmanager_p.h \
    $$PWD/connman/connmancommon.h
