TEMPLATE = subdirs
CONFIG += ordered

qtHaveModule(bluetooth): SUBDIRS += bluetoothsettings

SUBDIRS += \
    networksettings \
    localesettings \
    displaysettings \
    timedatesettings \
    localdevice \
    settingsui/settingsuiplugin \
    settingsui/qtbuttonimageproviderplugin \
    settingsui \
    imports \
    doc \
