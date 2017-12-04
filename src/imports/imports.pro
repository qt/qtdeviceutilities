TEMPLATE = subdirs

SUBDIRS += localdevice \
    displaysettings

qtHaveModule(bluetoothsettings): SUBDIRS += bluetoothsettings
qtHaveModule(networksettings): SUBDIRS += networksettings
qtHaveModule(localesettings): SUBDIRS += localesettings
qtHaveModule(timedatesettings): SUBDIRS += timedatesettings
