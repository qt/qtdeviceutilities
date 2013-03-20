TEMPLATE = lib
TARGET  = exampleplatforminputcontextplugin
TARGETPATH = $$[QT_INSTALL_PLUGINS]/platforminputcontexts

target.path = $$TARGETPATH

QT += quick gui gui-private

CONFIG += plugin

SOURCES += platforminputcontext.cpp \
           declarativeinputcontext.cpp \
           plugin.cpp

HEADERS += platforminputcontext.h \
           declarativeinputcontext.h

OTHER_FILES = $$$PWD/platforminputcontext.json

INSTALLS += target
OTHER += platforminputcontext.json
