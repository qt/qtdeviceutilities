CXX_MODULE = qml
QT += qml quick network
TARGET = qwifimodule
TARGETPATH = Qt/labs/wifi
IMPORT_VERSION = 0.1

HEADERS += \
    qwifimanager.h \
    qwifinetwork.h \
    qwifinetworklistmodel.h

SOURCES += \
    pluginmain.cpp \
    qwifimanager.cpp \
    qwifinetwork.cpp \
    qwifinetworklistmodel.cpp

android: {
    LIBS += -lhardware_legacy -lcutils
    DEFINES += FORCE_MOC
} else {
    DEFINES += CONFIG_CTRL_IFACE \
               CONFIG_CTRL_IFACE_UNIX

    HEADERS += qwifi_elinux.h
    SOURCES += \
           qwifi_elinux.cpp \
           $$[QT_SYSROOT]/usr/include/wpa-supplicant/wpa_ctrl.c \
           $$[QT_SYSROOT]/usr/include/wpa-supplicant/os_unix.c
}

load(qml_plugin)
