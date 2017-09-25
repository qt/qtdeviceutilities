QT += qml quick
CONFIG += no_cxx_module

TARGET = qtbuttonimageproviderplugin
TARGETPATH = QtDeviceUtilities/QtButtonImageProvider

SOURCES += \
    qtbuttonimageprovider.cpp

HEADERS += \
    qtbuttonimageprovider.h

RESOURCES += \
    plugin.qrc

load(qml_plugin)
