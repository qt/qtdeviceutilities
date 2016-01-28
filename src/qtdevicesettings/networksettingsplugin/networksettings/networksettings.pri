QT       += core network dbus

INCLUDEPATH += $${PWD}/connman
INCLUDEPATH += $${PWD}

DBUS_INTERFACES = \
    networksettings/connman/connman_manager.xml \
    networksettings/connman/connman_service.xml \
    networksettings/connman/connman_technology.xml

SOURCES += \
    $$PWD/qnetworksettingsinterfacemodel.cpp \
    $$PWD/qnetworksettingsmanager.cpp \
    $$PWD/qnetworksettingsaddressmodel.cpp \
    $$PWD/qnetworksettingsservicemodel.cpp \
    $$PWD/qnetworksettingsservice.cpp \
    $$PWD/qnetworksettingsuseragent.cpp \
    $$PWD/qnetworksettingsinterface.cpp \
    $$PWD/connman/qnetworksettingsinterface_p.cpp \
    $$PWD/connman/qnetworksettingsservice_p.cpp \
    $$PWD/connman/qnetworksettingsuseragent_p.cpp \
    $$PWD/connman/qnetworksettingsmanager_p.cpp \
    $$PWD/connman/connmancommon.cpp

HEADERS  += \
    $$PWD/qnetworksettingsinterfacemodel.h \
    $$PWD/qnetworksettings.h \
    $$PWD/qnetworksettingsmanager.h \
    $$PWD/qnetworksettingsaddressmodel.h \
    $$PWD/qnetworksettingsservicemodel.h \
    $$PWD/qnetworksettingsservice.h \
    $$PWD/qnetworksettingsuseragent.h \
    $$PWD/qnetworksettingsinterface.h \
    $$PWD/connman/qnetworksettingsinterface_p.h \
    $$PWD/connman/qnetworksettingsservice_p.h \
    $$PWD/connman/qnetworksettingsuseragent_p.h \
    $$PWD/connman/qnetworksettingsmanager_p.h \
    $$PWD/connman/connmancommon.h
