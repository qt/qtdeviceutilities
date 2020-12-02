# Generated from networksettings.pro.

#####################################################################
## networksettingsplugin Plugin:
#####################################################################

qt_add_qml_module(networksettingsplugin
    URI "QtDeviceUtilities.NetworkSettings"
    VERSION "${CMAKE_PROJECT_VERSION}"
    CLASSNAME NetworkSettingsQmlPlugin
    SKIP_TYPE_REGISTRATION
    SOURCES
        plugin.cpp plugin.h
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
        networksettings
)

#### Keys ignored in scope 1:.:.:networksettings.pro:<TRUE>:
# QML_IMPORT_VERSION = "$$QT_VERSION"
# TARGETPATH = "QtDeviceUtilities/NetworkSettings"
