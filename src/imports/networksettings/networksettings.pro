QT += qml quick networksettings
CONFIG += no_cxx_module
QML_IMPORT_VERSION = $$QT_VERSION

TARGET = networksettingsplugin
TARGETPATH = QtDeviceUtilities/NetworkSettings

SOURCES += \
    plugin.cpp \

HEADERS += \
    plugin.h \

load(qml_plugin)

