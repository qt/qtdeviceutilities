TEMPLATE = lib
CONFIG += plugin
QT += qml

uri = com.theqtcompany.settings.common

DESTDIR = imports/Settings
TARGET  = qmlsettingsplugin

pluginfiles.files += \
    imports/Settings/qmldir \
    imports/Settings/TextLabel.qml \
    imports/Settings/IconButton.qml \
    imports/Settings/CustomCombobox.qml \
    imports/Settings/FlatStyledDropdownDelegate.qml

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

target.path = $$installPath
pluginfiles.path += $$installPath
INSTALLS += target pluginfiles

RESOURCES +=

DISTFILES +=

