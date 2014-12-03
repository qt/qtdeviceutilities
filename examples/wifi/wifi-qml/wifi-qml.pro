TEMPLATE = app

QT += qml quick

SOURCES += main.cpp

RESOURCES += qml.qrc

target.path = /data/user/qt
export(target.path)
INSTALLS += target
