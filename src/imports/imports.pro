TEMPLATE = subdirs
SUBDIRS += networksettings \
    localesettings \
    timedatesettings \
    localdevice \
    displaysettings

qtHaveModule(bluetooth): SUBDIRS += bluetoothsettings
