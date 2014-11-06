CXX_MODULE = qml
TARGET = b2qtwifiplugin
TARGETPATH = B2Qt/Wifi
IMPORT_VERSION = 1.0

QT += qml b2qtwifi

SOURCES += pluginmain.cpp

OTHER_FILES += qmldir

load(qml_plugin)
