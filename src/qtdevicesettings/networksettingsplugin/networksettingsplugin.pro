TEMPLATE = lib
TARGET = networksettingsplugin
QT += qml quick
CONFIG += qt plugin

uri = com.theqtcompany.settings.network

wpasupplicant {
    include(networksettings/wpasupplicant.pri)
}
else {
    include(networksettings/connman.pri)
}


INCLUDEPATH += $${PWD}/networksettings

# Input
SOURCES += \
    networksettingsplugin_plugin.cpp \
    networksettings/qnetworksettingsinterfacemodel.cpp \
    networksettings/qnetworksettingsmanager.cpp \
    networksettings/qnetworksettingsaddressmodel.cpp \
    networksettings/qnetworksettingsservicemodel.cpp \
    networksettings/qnetworksettingsservice.cpp \
    networksettings/qnetworksettingsuseragent.cpp \
    networksettings/qnetworksettingsinterface.cpp \

HEADERS += \
    networksettingsplugin_plugin.h \
    networksettings/qnetworksettingsinterfacemodel.h \
    networksettings/qnetworksettings.h \
    networksettings/qnetworksettingsmanager.h \
    networksettings/qnetworksettingsaddressmodel.h \
    networksettings/qnetworksettingsservicemodel.h \
    networksettings/qnetworksettingsservice.h \
    networksettings/qnetworksettingsuseragent.h \
    networksettings/qnetworksettingsinterface.h \

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir

pluginfiles.files += \
    imports/NetworkSettings/EditWiredSettings.qml \
    imports/NetworkSettings/NetworkDetails.qml \
    imports/NetworkSettings/WifiSettings.qml \
    imports/NetworkSettings/IpAddressTextField.qml \
    imports/NetworkSettings/NetworkSettings.qml \
    imports/NetworkSettings/WiredSettings.qml \
    imports/NetworkSettings/WifiSelectorDelegate.qml \
    imports/NetworkSettings/WifiSignalMonitor.qml \
    ../icons/WifiAnim_black_2x.png \
    ../icons/WifiAnim_qt_2x.png \
    ../icons/Wifi_lightgray_2x.png \

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

pluginfiles.path = $$installPath
qmldir.path = $$installPath
target.path = $$installPath
INSTALLS += target qmldir pluginfiles

RESOURCES += \
    icons.qrc

