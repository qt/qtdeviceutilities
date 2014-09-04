CONFIG += c++11
QT = core network
SOURCES += \
           main.cpp \
           tar.cpp \
           filewrapper.cpp \
           update.cpp

HEADERS += \
           tar.h \
           filewrapper.h \
           update.h

LIBS += -lcrypto
INSTALLS += target
target.path = /usr/bin
