TEMPLATE = lib
CONFIG += plugin
QT += qml bluetooth
uri = com.theqtcompany.settings.bluetooth

TARGET  = btsettingsplugin

include(bluez/bluez.pri)

HEADERS = \
    bluetoothdevice.h \
    discoverymodel.h

SOURCES += plugin.cpp \
    bluetoothdevice.cpp \
    discoverymodel.cpp


pluginfiles.files += \
    qmldir \


installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

target.path = $$installPath
pluginfiles.path += $$installPath
INSTALLS += target pluginfiles


RESOURCES += \
    icons.qrc \
    qml.qrc

