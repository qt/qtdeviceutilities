add_subdirectory(networksettings)
add_subdirectory(doc)

if(TARGET Qt::Qml)
    add_subdirectory(networksettingsqml)
endif()
