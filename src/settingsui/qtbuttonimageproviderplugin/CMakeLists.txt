#####################################################################
## qtbuttonimageproviderplugin Plugin:
#####################################################################

qt_internal_add_qml_module(qtbuttonimageproviderplugin
    URI "QtDeviceUtilities.QtButtonImageProvider"
    VERSION "${CMAKE_PROJECT_VERSION}"
    CLASS_NAME QtButtonImageProviderQmlPlugin
    PLUGIN_TARGET qtbuttonimageproviderplugin
    NO_GENERATE_PLUGIN_SOURCE
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_QMLTYPES
    INSTALL_SOURCE_QMLTYPES "plugins.qmltypes"
    SOURCES
        plugin.cpp
        qtbuttonimageprovider.cpp qtbuttonimageprovider.h
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
)

# Resources:
set(qtbuttonimageprovider_resource_files
    "QtButton.qml"
)

qt_internal_add_resource(qtbuttonimageproviderplugin "qtbuttonimageprovider"
    PREFIX
        "/"
    FILES
        ${qtbuttonimageprovider_resource_files}
)
