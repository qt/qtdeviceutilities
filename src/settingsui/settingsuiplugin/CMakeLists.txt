# Generated from settingsuiplugin.pro.

#####################################################################
## settingsuiplugin Plugin:
#####################################################################

qt_add_qml_module(settingsuiplugin
    URI "QtDeviceUtilities.SettingsUI"
    VERSION "${CMAKE_PROJECT_VERSION}"
    CLASSNAME SettingsUIQmlPlugin
    SKIP_TYPE_REGISTRATION
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
    "icons/Audio_qt_6x.png"
    "icons/Bluetooth_qt_1x.png"
    "icons/Bluetooth_qt_6x.png"
    "icons/Camcorder_qt_1x.png"
    "icons/Camera_qt_1x.png"
    "icons/Chevron-left_black.png"
    "icons/Chevron-left_black_6x.png"
    "icons/Clock_qt_1x.png"
    "icons/Clock_qt_6x.png"
    "icons/Display_qt_6x.png"
    "icons/Flag_qt_6x.png"
    "icons/FloatingButton_Active.png"
    "icons/FloatingButton_Available.png"
    "icons/FloatingButton_Unavailable.png"
    "icons/Headphones_qt_1x.png"
    "icons/Heartbeat_qt_1x.png"
    "icons/Keyboard_qt_1x.png"
    "icons/Laptop_qt_1x.png"
    "icons/Microphone_qt_1x.png"
    "icons/MobilePhone_qt_1x.png"
    "icons/Mouse_qt_1x.png"
    "icons/Power_black_6x.png"
    "icons/Tablet_qt_1x.png"
    "icons/Volume_down_qt_1x.png"
    "icons/Volume_mute_qt_1x.png"
    "icons/Volume_up_qt_1x.png"
    "icons/WifiAnim_black_2x.png"
    "icons/Wifi_lightgray_2x.png"
    "icons/World_qt_6x.png"
    "newIcons/back_icon.svg"
    "newIcons/close_icon.svg"
    "newIcons/detail_icon.svg"
    "newIcons/grid_icon.svg"
    "newIcons/header_toggle_icon.svg"
    "newIcons/info_icon.svg"
    "newIcons/mouse_icon.svg"
    "newIcons/mute_icon.svg"
    "newIcons/network_icon.svg"
    "newIcons/pause_icon.svg"
    "newIcons/play_icon.svg"
    "newIcons/qt_logo_green_rgb.svg"
    "newIcons/settings_icon.svg"
    "newIcons/settingsmenu_launcher_icon.svg"
    "newIcons/stop_icon.svg"
    "newIcons/touch_icon.svg"
    "newIcons/volume_icon.svg"
    "pluginMain.qml"
    "settingsview.xml"
)

qt_add_resource(settingsuiplugin "settingsui"
    PREFIX
        "/"
    FILES
        ${settingsui_resource_files}
)


#### Keys ignored in scope 1:.:.:settingsuiplugin.pro:<TRUE>:
# DISTFILES = "qmldir"
# QML_IMPORT_VERSION = "$$QT_VERSION"
# TARGETPATH = "QtDeviceUtilities/SettingsUI"
