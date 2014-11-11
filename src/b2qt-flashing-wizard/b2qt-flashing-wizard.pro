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
