#####################################################################
## settingsui Binary:
#####################################################################

qt_internal_add_executable(settingsui
    GUI
    SOURCES
        main.cpp
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::Qml
        Qt::Quick
)

# Resources:
set(settingsuiapp_resource_files
    "AutoScroller.qml"
    "ViewSettings.qml"
    "common/CustomSwitch.qml"
    "common/HandwritingModeButton.qml"
    "main.qml"
    "network/AddressListEntry.qml"
    "network/ComboBoxEntry.qml"
    "network/EditWiredSettings.qml"
    "network/IpAddressTextField.qml"
    "network/NetworkDetails.qml"
    "network/NetworkListView.qml"
    "network/NetworkSettings.qml"
    "network/PassphraseEnter.qml"
    "network/WifiSelectorDelegate.qml"
    "network/WifiSettings.qml"
    "network/WifiSignalMonitor.qml"
    "network/WiredSettings.qml"
    "settingsuiplugin/SettingsDelegate.qml"
    "settingsuiplugin/SettingsHeader.qml"
    "settingsuiplugin/SettingsTitleItem.qml"
    "settingsuiplugin/pluginMain.qml"
)

qt_internal_add_resource(settingsui "settingsuiapp"
    PREFIX
        "/"
    FILES
        ${settingsuiapp_resource_files}
)

qt_internal_extend_target(settingsui CONDITION static
    PUBLIC_LIBRARIES
        Qt::Svg
)
