QT       += core network

INCLUDEPATH += $${PWD}/wpasupplicant

HEADERS  += \
    $$PWD/wpasupplicant/qnetworksettingsinterface_p.h \
    $$PWD/wpasupplicant/qnetworksettingsmanager_p.h \
    $$PWD/wpasupplicant/qwifisupplicant_p.h \
    $$PWD/wpasupplicant/qwificontroller_p.h \
    $$PWD/wpasupplicant/qnetworksettingsservice_p.h \
    $$PWD/wpasupplicant/qnetworksettingsuseragent_p.h \
    $$PWD/wpasupplicant/qwifidevice.h

SOURCES += \
    $$PWD/wpasupplicant/qnetworksettingsinterface_p.cpp \
    $$PWD/wpasupplicant/qnetworksettingsmanager_p.cpp \
    $$[QT_SYSROOT]/usr/include/wpa-supplicant/wpa_ctrl.c \
    $$[QT_SYSROOT]/usr/include/wpa-supplicant/os_unix.c \
    $$PWD/wpasupplicant/qwifisupplicant.cpp \
    $$PWD/wpasupplicant/qwificontroller.cpp \
    $$PWD/wpasupplicant/qnetworksettingsservice_p.cpp \
    $$PWD/wpasupplicant/qnetworksettingsuseragent_p.cpp \
    $$PWD/wpasupplicant/qwifidevice.cpp

DEFINES += \
       CONFIG_CTRL_IFACE \
       CONFIG_CTRL_IFACE_UNIX

