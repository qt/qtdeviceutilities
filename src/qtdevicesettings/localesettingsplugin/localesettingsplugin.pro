TEMPLATE = lib
CONFIG += plugin
QT += qml dbus

uri = com.theqtcompany.settings.locale

DBUS_INTERFACES = locale.xml

DESTDIR = imports/Locale
TARGET  = localesettingsplugin

HEADERS = \
    systemlocale.h \
    systemlocale_p.h \
    localemodel.h \
    localefiltermodel.h
SOURCES += plugin.cpp \
    systemlocale.cpp \
    localemodel.cpp \
    localefiltermodel.cpp

pluginfiles.files += \
    qmldir \
    Language.qml \
    RegionSelect.qml

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

target.path = $$installPath
pluginfiles.path += $$installPath
INSTALLS += target pluginfiles


RESOURCES += \
    icons.qrc

