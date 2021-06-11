#####################################################################
## networksettingsplugin Plugin:
#####################################################################

qt_internal_add_qml_module(networksettingsplugin
    URI "QtDeviceUtilities.NetworkSettings"
    VERSION "${CMAKE_PROJECT_VERSION}"
    CLASS_NAME NetworkSettingsQmlPlugin
    PLUGIN_TARGET networksettingsplugin
    NO_GENERATE_PLUGIN_SOURCE
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_QMLTYPES
    INSTALL_SOURCE_QMLTYPES "plugins.qmltypes"
    SOURCES
        plugin.cpp plugin.h
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
        Qt::NetworkSettings
)
