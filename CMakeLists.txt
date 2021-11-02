cmake_minimum_required(VERSION 3.15.0)

include(.cmake.conf)
project(QtDeviceUtilities
    VERSION "${QT_REPO_MODULE_VERSION}"
    DESCRIPTION "Qt Device Utilities Libraries"
    HOMEPAGE_URL "https://qt.io/"
    LANGUAGES CXX C
)

find_package(Qt6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS BuildInternals Core)
find_package(Qt6 ${PROJECT_VERSION} QUIET CONFIG OPTIONAL_COMPONENTS Gui Network Quick Qml QuickControls2 DBus)

macro(assertTargets)
    foreach(qtTarget IN ITEMS ${ARGN})
        if(NOT TARGET Qt::${qtTarget})
           message(NOTICE "Skipping the build as the condition \"TARGET Qt::${qtTarget}\" is not met.")
           return()
        endif()
    endforeach()
endmacro()

assertTargets(Quick DBus)

if(NOT LINUX)
    message(NOTICE "Skipping the build as the condition \"LINUX\" is not met.")
    return()
endif()
if(WASM)
    message(NOTICE "Skipping the build as the condition \"NOT WASM\" is not met.")
    return()
endif()

qt_build_repo()
