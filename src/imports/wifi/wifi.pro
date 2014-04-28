CXX_MODULE = qml
QT += qml quick network
TARGET = qwifimodule
TARGETPATH = Qt/labs/wifi
IMPORT_VERSION = 0.1

SOURCES += \
    pluginmain.cpp \
    qwifimanager.cpp \
    qwifinetwork.cpp \
    qwifinetworklistmodel.cpp

HEADERS += \
    qwifimanager.h \
    qwifinetwork.h \
    qwifinetworklistmodel.h

LIBS += -lhardware_legacy -lcutils

load(qml_plugin)

