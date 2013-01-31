CXX_MODULE = qml
TARGET  = qtdroidutilsplugin
TARGETPATH = QtDroid/Utils
IMPORT_VERSION = 1.0

QT += qml

SOURCES += plugin.cpp

### kludge
INCLUDEPATH += $$PWD/../../../include
LIBS += -L$$PWD/../../../lib -lQt5DroidUtils

load(qml_plugin)
