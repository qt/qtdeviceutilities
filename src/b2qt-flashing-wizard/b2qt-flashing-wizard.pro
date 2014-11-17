QT += widgets
CONFIG += c++11

SOURCES += \
          actor.cpp \
          commit_page.cpp \
          common.cpp \
          device_page.cpp \
          disk_page.cpp \
          elevator.cpp \
          finish_page.cpp \
          introduction_page.cpp \
          main.cpp \
          mainwindow.cpp \
          platform_page.cpp \
          progress_page.cpp \
          scriptwriter.cpp \

HEADERS += \
          actor.h \
          commit_page.h \
          common.h \
          device_page.h \
          disk_page.h \
          elevator.h \
          finish_page.h \
          introduction_page.h \
          mainwindow.h \
          platform_page.h \
          progress_page.h \
          scriptwriter.h \

RESOURCES += b2qt-flashing-wizard.qrc
INSTALLS += target
target.path = /bin

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
