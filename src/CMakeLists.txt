# Generated from src.pro.

add_subdirectory(settingsui/settingsuiplugin)
add_subdirectory(settingsui/qtbuttonimageproviderplugin)
add_subdirectory(settingsui)
add_subdirectory(imports)
add_subdirectory(doc)
if(TARGET Qt::DBus)
    add_subdirectory(networksettings)
endif()
