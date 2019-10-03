load(qt_build_config)

TARGET = QtNetworkSettings
VERSION = 1.0

QT = core network

MODULE = networksettings
load(qt_module)
include(connman.pri)

# Input
SOURCES += \
    qnetworksettingsinterfacemodel.cpp \
    qnetworksettings.cpp \
    qnetworksettingsmanager.cpp \
    qnetworksettingsaddressmodel.cpp \
    qnetworksettingsservicemodel.cpp \
    qnetworksettingsservice.cpp \
    qnetworksettingsuseragent.cpp \
    qnetworksettingsinterface.cpp \

HEADERS += \
    qnetworksettingsinterfacemodel.h \
    qnetworksettings.h \
    qnetworksettingsmanager.h \
    qnetworksettingsaddressmodel.h \
    qnetworksettingsservicemodel.h \
    qnetworksettingsservice.h \
    qnetworksettingsuseragent.h \
    qnetworksettingsinterface.h \
