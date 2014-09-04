TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    utils \
    imports \
    doc \
    plugins \
    b2qt-update-util


android: SUBDIRS += doppelganger qt_hw_init qconnectivity
