TARGET = QtDroidUtils
VERSION = 5
CONFIG += dll warn_on

QT = core network
#QT = core-private gui-private qml-private quick-private
#QT_PRIVATE = v8-private

#DEFINES   += QT_NO_URL_CAST_FROM_STRING QT_NO_INTEGER_EVENT_COORDINATES

MODULE = droidutils
load(qt_module)

android: LIBS += -lhardware -lcutils

HEADERS += \
    $$PWD/qdroidutils.h

SOURCES += \
    $$PWD/qdroidutils.cpp
