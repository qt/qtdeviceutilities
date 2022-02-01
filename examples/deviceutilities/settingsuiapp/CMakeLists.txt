cmake_minimum_required(VERSION 3.16)

project(settingsapp VERSION 1.0 LANGUAGES CXX)

if(NOT DEFINED INSTALL_EXAMPLESDIR)
    set(INSTALL_EXAMPLESDIR "${CMAKE_INSTALL_PREFIX}/share/examples")
endif()
set(INSTALL_EXAMPLEDIR "${INSTALL_EXAMPLESDIR}/deviceutilities/settingsapp")

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Qml Quick)

add_subdirectory(DeviceUtilities/SettingsUI)
add_subdirectory(DeviceUtilities/QtButtonImageProvider)

set(qml_files
    "main.qml"
    "AutoScroller.qml"
    "HandwritingModeButton.qml"
)

set(images
    "images/FloatingButton_Active.png"
    "images/FloatingButton_Available.png"
    "images/FloatingButton_Unavailable.png"
)

qt_add_executable(settingsapp
    main.cpp
)

qt_add_qml_module(settingsapp
    URI DeviceUtilities
    VERSION 1.0
    QML_FILES
        ${qml_files}
    SOURCES
        applicationsettings.cpp
        applicationsettings.h
    RESOURCES
        ${images}
)

target_link_libraries(settingsapp PRIVATE
    Qt::Core
    Qt::Gui
)

install(TARGETS settingsapp
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
