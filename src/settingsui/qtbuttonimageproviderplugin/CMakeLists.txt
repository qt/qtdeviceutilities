# Generated from qtbuttonimageproviderplugin.pro.

#####################################################################
## qtbuttonimageproviderplugin Plugin:
#####################################################################

qt_add_qml_module(qtbuttonimageproviderplugin
    URI "QtDeviceUtilities.QtButtonImageProvider"
    VERSION "${CMAKE_PROJECT_VERSION}"
    CLASSNAME QtButtonImageProviderQmlPlugin
    SKIP_TYPE_REGISTRATION
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

qt_add_resource(qtbuttonimageproviderplugin "qtbuttonimageprovider"
    PREFIX
        "/"
    FILES
        ${qtbuttonimageprovider_resource_files}
)


#### Keys ignored in scope 1:.:.:qtbuttonimageproviderplugin.pro:<TRUE>:
# QML_IMPORT_VERSION = "$$QT_VERSION"
# TARGETPATH = "QtDeviceUtilities/QtButtonImageProvider"
