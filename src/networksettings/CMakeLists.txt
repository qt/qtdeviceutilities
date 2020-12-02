# Generated from networksettings.pro.

#####################################################################
## NetworkSettings Module:
#####################################################################

qt_add_module(NetworkSettings
    SOURCES
        connman/connmancommon.cpp connman/connmancommon.h
        connman/qnetworksettingsinterface_p.cpp connman/qnetworksettingsinterface_p.h
        connman/qnetworksettingsmanager_p.cpp connman/qnetworksettingsmanager_p.h
        connman/qnetworksettingsservice_p.cpp connman/qnetworksettingsservice_p.h
        connman/qnetworksettingsuseragent_p.cpp connman/qnetworksettingsuseragent_p.h
        qnetworksettings.cpp qnetworksettings.h
        qnetworksettingsaddressmodel.cpp qnetworksettingsaddressmodel.h
        qnetworksettingsinterface.cpp qnetworksettingsinterface.h
        qnetworksettingsinterfacemodel.cpp qnetworksettingsinterfacemodel.h
        qnetworksettingsmanager.cpp qnetworksettingsmanager.h
        qnetworksettingsservice.cpp qnetworksettingsservice.h
        qnetworksettingsservicemodel.cpp qnetworksettingsservicemodel.h
        qnetworksettingsuseragent.cpp qnetworksettingsuseragent.h
    DBUS_INTERFACE_SOURCES
        connman/connman_manager.xml
        connman/connman_service.xml
        connman/connman_technology.xml
    DBUS_INTERFACE_FLAGS  # special case
        -iconnmancommon.h # special case
    INCLUDE_DIRECTORIES
        connman
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::DBus
        Qt::Network
)

#### Keys ignored in scope 1:.:.:networksettings.pro:<TRUE>:
# MODULE = "networksettings"
# VERSION = "1.0"
