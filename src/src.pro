TEMPLATE = subdirs
CONFIG += ordered

qtHaveModule(dbus) {
    qtHaveModule(bluetooth): SUBDIRS += bluetoothsettings
    SUBDIRS += \
        networksettings \
        localesettings \
        timedatesettings \
}

SUBDIRS += \
    displaysettings \
    localdevice \
    settingsui/settingsuiplugin \
    settingsui/qtbuttonimageproviderplugin \
    settingsui \
    imports \
    doc \
