#-------------------------------------------------
#
# Project created by QtCreator 2015-11-03T13:52:45
#
#-------------------------------------------------

QT       += core network dbus

INCLUDEPATH += $${PWD}/connman
INCLUDEPATH += $${PWD}

DBUS_INTERFACES = \
    networksettings/connman/connman_manager.xml \
    networksettings/connman/connman_service.xml \
    networksettings/connman/connman_technology.xml


SOURCES += networksettings/networksettingsmanager.cpp \
    networksettings/connman/networksettingsmanager_linux.cpp \
    networksettings/networkaddressmodel.cpp \
    networksettings/networksmodel.cpp

HEADERS  += \
    networksettings/networkconfig.h \
    networksettings/connman/networksettingsmanager_linux_p.h \
    networksettings/networkaddressmodel.h \
    networksettings/networksettingsmanager.h \
    networksettings/networksmodel.h
