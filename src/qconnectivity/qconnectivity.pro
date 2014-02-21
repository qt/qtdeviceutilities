QT       += core network
QT       -= gui

TARGET = qconnectivity
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -lcutils -lnetutils
SOURCES += main.cpp

load(qt_tool)
