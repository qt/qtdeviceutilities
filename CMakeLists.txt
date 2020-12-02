# Generated from qtdeviceutilities.pro.

cmake_minimum_required(VERSION 3.15.0)

include(.cmake.conf)
project(QtDeviceUtilities # special case
    VERSION "${QT_REPO_MODULE_VERSION}"
    DESCRIPTION "Qt Device Utilities Libraries" # special case
    HOMEPAGE_URL "https://qt.io/"
    LANGUAGES CXX C
)

find_package(Qt6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS BuildInternals Quick DBus) # special case
find_package(Qt6 ${PROJECT_VERSION} CONFIG OPTIONAL_COMPONENTS)

if(NOT LINUX)
    message(NOTICE "Skipping the build as the condition \"LINUX\" is not met.")
    return()
endif()
if(NOT TARGET Qt::Quick)
    message(NOTICE "Skipping the build as the condition \"TARGET Qt::Quick\" is not met.")
    return()
endif()
qt_build_repo()
