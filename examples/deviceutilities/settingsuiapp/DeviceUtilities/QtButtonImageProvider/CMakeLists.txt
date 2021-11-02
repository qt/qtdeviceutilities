# There are a number of options to qt_add_qml_module that start with capital NO.
# The most prominent NO option is probably NO_GENERATE_PLUGIN_SOURCE.
# Unfortunately image providers still need to be configured per QML engine.
# Therefore, if you bundle an image provider in your QML module, you need to implement
# the QQmlEngineExtensionPlugin::initializeEngine() method.
# This, in turn, makes it necessary to write your own plugin.
# In this example the plugin is in the QtButtonImageProvider class.

qt_add_qml_module(qtbuttonimageproviderplugin
    URI "DeviceUtilities.QtButtonImageProvider"
    VERSION 1.0
    PLUGIN_TARGET qtbuttonimageproviderplugin
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_PLUGIN_SOURCE
    CLASS_NAME QtButtonImageProviderPlugin
    SOURCES
        plugin.cpp plugin_p.h
        qtbuttonimageprovider.cpp qtbuttonimageprovider.h
    QML_FILES
        "QtButton.qml"
)

target_link_libraries(qtbuttonimageproviderplugin PRIVATE
    Qt::Quick
)

install(TARGETS qtbuttonimageproviderplugin
    DESTINATION "${INSTALL_EXAMPLEDIR}/DeviceUtilities/QtButtonImageProvider"
)
install(FILES $<TARGET_FILE_DIR:qtbuttonimageproviderplugin>/qmldir
    DESTINATION "${INSTALL_EXAMPLEDIR}/DeviceUtilities/QtButtonImageProvider"
)
