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

# Find out git hash
unix:system(which git):HAS_GIT=TRUE
win32:system(where git.exe):HAS_GIT=TRUE
contains(HAS_GIT, TRUE) {
    GIT_HASH=$$system(git log -1 --format=%H)
    !system(git diff-index --quiet HEAD): GIT_HASH="$$GIT_HASH-dirty"
    GIT_VERSION=$$system(git describe --tags --exact-match)
    isEmpty(GIT_VERSION) : GIT_VERSION="unknown"
} else {
    GIT_HASH="unknown"
    GIT_VERSION="unknown"
}

isEmpty(GIT_VERSION) : error("No suitable tag found")
isEmpty(GIT_HASH) : error("No hash available")

DEFINES+="GIT_HASH=\\\"$$GIT_HASH\\\""
DEFINES+="GIT_VERSION=\\\"$$GIT_VERSION\\\""
