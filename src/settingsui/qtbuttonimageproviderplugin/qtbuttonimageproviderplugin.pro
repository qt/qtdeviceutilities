QT += qml quick
CONFIG += no_cxx_module

TARGET = qtbuttonimageproviderplugin
TARGETPATH = QtDeviceUtilities/QtButtonImageProvider

SOURCES += \
    qtbuttonimageprovider.cpp \
    plugin.cpp

HEADERS += \
    qtbuttonimageprovider.h

RESOURCES += \
    qtbuttonimageprovider.qrc

load(qml_plugin)
