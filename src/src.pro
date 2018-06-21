TEMPLATE = subdirs
CONFIG += ordered

qtHaveModule(dbus) {
    SUBDIRS += \
        networksettings \
        localesettings \
        timedatesettings \

    qtHaveModule(bluetooth): SUBDIRS += bluetoothsettings
}

SUBDIRS += \
    displaysettings \
    localdevice \
    settingsui/settingsuiplugin \
    settingsui/qtbuttonimageproviderplugin \
    settingsui \
    imports \
    doc \
