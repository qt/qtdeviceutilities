TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    networksettings \
    localesettings \
    displaysettings \
    timedatesettings \
    localdevice \
    settingsui/settingsuiplugin \
    settingsui \
    imports \
    doc \

qtHaveModule(bluetooth): SUBDIRS += bluetoothsettings
