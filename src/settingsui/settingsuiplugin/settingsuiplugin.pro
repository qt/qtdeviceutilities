QT += qml quick
CONFIG += no_cxx_module

TARGET = settingsuiplugin
TARGETPATH = QtDeviceUtilities/SettingsUI

SOURCES += \
    plugin.cpp

DISTFILES += \
    qmldir

RESOURCES += \
    settingsui.qrc

load(qml_plugin)
