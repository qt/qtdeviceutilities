#####################################################################
## settingsuiplugin Plugin:
#####################################################################

qt_internal_add_qml_module(settingsuiplugin
    URI "QtDeviceUtilities.SettingsUI"
    VERSION "${CMAKE_PROJECT_VERSION}"
    CLASS_NAME SettingsUIQmlPlugin
    PLUGIN_TARGET settingsuiplugin
    NO_GENERATE_PLUGIN_SOURCE
    NO_PLUGIN_OPTIONAL
    NO_GENERATE_QMLTYPES
    INSTALL_SOURCE_QMLTYPES "plugins.qmltypes"
    SOURCES
        plugin.cpp
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::Qml
        Qt::Quick
)

# Resources:
set(settingsui_resource_files
    "../common/CustomSwitch.qml"
    "../common/CustomTableView.qml"
    "../common/HandwritingModeButton.qml"
    "../network/AddressListEntry.qml"
    "../network/ComboBoxEntry.qml"
    "../network/EditWiredSettings.qml"
    "../network/IpAddressTextField.qml"
    "../network/NetworkDetails.qml"
    "../network/NetworkListView.qml"
    "../network/NetworkSettings.qml"
    "../network/PassphraseEnter.qml"
    "../network/WifiSelectorDelegate.qml"
    "../network/WifiSettings.qml"
    "../network/WifiSignalMonitor.qml"
    "../network/WiredSettings.qml"
    "SettingsDelegate.qml"
    "SettingsHeader.qml"
    "SettingsTitleItem.qml"
    "icons/Alert_yellow_1x.png"
    "icons/FloatingButton_Active.png"
    "icons/FloatingButton_Available.png"
    "icons/FloatingButton_Unavailable.png"
    "icons/WifiAnim_black_2x.png"
    "icons/Wifi_lightgray_2x.png"
    "newIcons/back_icon.svg"
    "newIcons/network_icon.svg"
    "pluginMain.qml"
)

qt_internal_add_resource(settingsuiplugin "settingsui"
    PREFIX
        "/"
    FILES
        ${settingsui_resource_files}
)
