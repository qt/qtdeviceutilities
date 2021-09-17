#####################################################################
## qtbuttonimageproviderplugin Plugin:
#####################################################################

qt_internal_add_qml_module(qtbuttonimageproviderplugin
    URI "QtDeviceUtilities.QtButtonImageProvider"
    VERSION "${CMAKE_PROJECT_VERSION}"
    CLASS_NAME QtButtonImageProviderQmlPlugin
    PLUGIN_TARGET qtbuttonimageproviderplugin
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    CLASS_NAME QtButtonImageProviderPlugin
    PAST_MAJOR_VERSIONS 1
    DEPENDENCIES
        QtQml
    SOURCES
        plugin.cpp plugin_p.h
        qtbuttonimageprovider.cpp qtbuttonimageprovider.h
    LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
    QML_FILES
        "QtButton.qml"
)
