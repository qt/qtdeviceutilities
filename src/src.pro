TEMPLATE = subdirs
CONFIG += ordered

qtHaveModule(dbus) {
    SUBDIRS += \
        networksettings \

}

SUBDIRS += \
    settingsui/settingsuiplugin \
    settingsui/qtbuttonimageproviderplugin \
    settingsui \
    imports \
    doc \
