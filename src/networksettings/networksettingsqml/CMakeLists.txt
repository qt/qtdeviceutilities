qt_internal_add_qml_module(networksettingsqml
    URI "QtDeviceUtilities.NetworkSettings"
    VERSION "${CMAKE_PROJECT_VERSION}"
    PAST_MAJOR_VERSIONS 1
    PLUGIN_TARGET networksettingsqml
    DEPENDENCIES
        QtQml
    SOURCES
        qnetworksettingsforeign.h qnetworksettingsforeign.cpp
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Qml
        Qt::NetworkSettings
)
