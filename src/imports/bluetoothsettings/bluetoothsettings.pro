TEMPLATE = lib
CONFIG += plugin
QT += qml bluetooth bluetoothsettings
uri = QtDeviceUtilities.BluetoothSettings

TARGET  = btsettingsplugin

pluginfiles.files += \
    qmldir \

SOURCES += plugin.cpp

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

target.path = $$installPath
pluginfiles.path += $$installPath
INSTALLS += target pluginfiles
