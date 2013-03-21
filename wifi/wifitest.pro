#-------------------------------------------------
#
# Project created by QtCreator 2013-03-11T12:29:12
#
#-------------------------------------------------

QT = core gui qml quick

TARGET = wifitest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cpp \
    qwifimanager.cpp \
    qwifinetwork.cpp \
    qwifinetworklist.cpp

HEADERS += \
    qwifimanager.h \
    qwifinetwork.h \
    qwifinetworklist.h

LIBS += -lhardware_legacy -lcutils

OTHER_FILES += \
    main.qml
