QT += qml displaysettings

CONFIG += no_cxx_module

TARGET  = qmldisplaysettingsplugin
TARGETPATH = QtDeviceUtilities/DisplaySettings

SOURCES += plugin.cpp \

load(qml_plugin)
