set(qml_files
    "Globals.qml"
    "SettingsUI.qml"
)

# Resources:
set(images
    "icons/back.svg"
    "icons/network.svg"
)

set_source_files_properties(Globals.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE true
)

find_package(Qt6 COMPONENTS NetworkSettings QuickControls2)

add_subdirectory(Network)

qt_add_qml_module(settingsui
    URI "DeviceUtilities.SettingsUI"
    VERSION 1.0
    QML_FILES
        ${qml_files}
    RESOURCES
        ${images}
)

target_link_libraries(settingsui PRIVATE
    Qt::NetworkSettings
    Qt::QuickControls2
)

set_target_properties(settingsuiplugin PROPERTIES
    INSTALL_RPATH $ORIGIN/../..:$ORIGIN/../../${CMAKE_INSTALL_LIBDIR}
)

install(TARGETS settingsui
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)

install(TARGETS settingsuiplugin
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}/DeviceUtilities/SettingsUI"
)
install(FILES $<TARGET_FILE_DIR:settingsuiplugin>/qmldir
    DESTINATION "${INSTALL_EXAMPLEDIR}/DeviceUtilities/SettingsUI"
)
