TEMPLATE = app
TARGET = qt_hw_init
QT -= gui
QT += droidutils

SOURCES += main.cpp

LIBS += -lQt5DroidUtils

load(qt_tool)
