TEMPLATE = lib
TARGET = networksettingsplugin
QT += qml quick networksettings
CONFIG += qt plugin

uri = QtDeviceUtilities.NetworkSettings

SOURCES += \
    plugin.cpp \

HEADERS += \
    plugin.h \

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir

pluginfiles.files += \

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

pluginfiles.path = $$installPath
qmldir.path = $$installPath
target.path = $$installPath
INSTALLS += target qmldir pluginfiles

