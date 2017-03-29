QT += qml quick networksettings

CONFIG += no_cxx_module

TARGET = networksettingsplugin
TARGETPATH = QtDeviceUtilities/NetworkSettings

SOURCES += \
    plugin.cpp \

HEADERS += \
    plugin.h \

load(qml_plugin)

