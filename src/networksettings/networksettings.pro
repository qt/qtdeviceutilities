load(qt_build_config)

TARGET = QtNetworkSettings
VERSION = 1.0
CONFIG += dll warn_on

QT = core network

MODULE = networksettings
load(qt_module)

connman {
    include(connman.pri)
}
else {
    include(wpasupplicant.pri)
}

# Input
SOURCES += \
    qnetworksettingsinterfacemodel.cpp \
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
