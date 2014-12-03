QT += core widgets b2qtwifi

TARGET = wifi-cpp
TEMPLATE = app

target.path = /data/user/qt/$$TARGET
INSTALLS   += target

SOURCES += main.cpp
