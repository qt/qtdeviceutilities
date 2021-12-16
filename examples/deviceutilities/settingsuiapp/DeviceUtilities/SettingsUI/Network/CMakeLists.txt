set(qml_files
    "CustomSwitch.qml"
    "CustomTableView.qml"
    "AddressListEntry.qml"
    "ComboBoxEntry.qml"
    "EditWiredSettings.qml"
    "IpAddressTextField.qml"
    "NetworkListView.qml"
    "NetworkSettings.qml"
    "PassphraseEnter.qml"
    "WifiSelectorDelegate.qml"
    "WifiSettings.qml"
    "WifiSignalMonitor.qml"
    "WiredSettings.qml"
)

# Resources:
set(images
    "icons/Alert_yellow_1x.png"
    "icons/WifiAnim_black_2x.png"
    "icons/Wifi_lightgray_2x.png"
)

find_package(Qt6 COMPONENTS NetworkSettings QuickControls2)

qt_add_qml_module(settingsuinetwork
    URI "DeviceUtilities.SettingsUI.Network"
    VERSION 1.0
    QML_FILES
        ${qml_files}
    RESOURCES
        ${images}
)

target_link_libraries(settingsuinetwork PRIVATE
    Qt::NetworkSettings
    Qt::QuickControls2
)

set_target_properties(settingsuinetworkplugin PROPERTIES
    INSTALL_RPATH $ORIGIN/../../..:$ORIGIN/../../../${CMAKE_INSTALL_LIBDIR}
)

install(TARGETS settingsuinetwork
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)

install(TARGETS settingsuinetworkplugin
    DESTINATION "${INSTALL_EXAMPLEDIR}/DeviceUtilities/SettingsUI/Network"
)
install(FILES $<TARGET_FILE_DIR:settingsuinetworkplugin>/qmldir
    DESTINATION "${INSTALL_EXAMPLEDIR}/DeviceUtilities/SettingsUI/Network"
)
