TEMPLATE = app
TARGET = virtualkeyboard
QT += qml quick
SOURCES += main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/qtdeclarative/demos/virtualkeyboard
qml.files = content virtualkeyboard.qml
qml.path = $$[QT_INSTALL_EXAMPLES]/qtdeclarative/demos/virtualkeyboard
INSTALLS += target qml
