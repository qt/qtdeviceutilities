QT += qml quick
CONFIG += no_cxx_module
QML_IMPORT_VERSION = $$QT_VERSION

TARGET = settingsuiplugin
TARGETPATH = QtDeviceUtilities/SettingsUI

SOURCES += \
    plugin.cpp

DISTFILES += \
    qmldir

RESOURCES += \
    settingsui.qrc

load(qml_plugin)
