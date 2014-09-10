CXX_MODULE = qml
TARGET  = b2qtutilsplugin
TARGETPATH = B2Qt/Utils
IMPORT_VERSION = 1.0

QT += qml b2qtutils

SOURCES += plugin.cpp

load(qml_plugin)
