load(qt_build_config)

TARGET = B2QtWifi
VERSION = 1.0
CONFIG += dll warn_on

QT += core network

MODULE = b2qtwifi
load(qt_module)

HEADERS += \
    $$PWD/qwifimanager.h \
    $$PWD/qwifimanager_p.h \
    $$PWD/qwifinetwork_p.h \
    $$PWD/qwifinetworklistmodel_p.h \
    $$PWD/qwificontroller_p.h \
    $$PWD/qwifidevice.h \
    $$PWD/qwificonfiguration.h \
    $$PWD/qwifiutils_p.h

SOURCES += \
    $$PWD/qwifimanager.cpp \
    $$PWD/qwifinetwork.cpp \
    $$PWD/qwifinetworklistmodel.cpp \
    $$PWD/qwificontroller.cpp \
    $$PWD/qwifidevice.cpp \
    $$PWD/qwificonfiguration.cpp \
    $$PWD/qwifiutils.cpp

android: {
    LIBS += -lhardware_legacy -lcutils
} else {
    DEFINES += \
           CONFIG_CTRL_IFACE \
           CONFIG_CTRL_IFACE_UNIX

    HEADERS += \
           $$PWD/qwifielinux_p.h

    SOURCES += \
           $$PWD/qwifielinux.cpp \
           $$[QT_SYSROOT]/usr/include/wpa-supplicant/wpa_ctrl.c \
           $$[QT_SYSROOT]/usr/include/wpa-supplicant/os_unix.c
}
