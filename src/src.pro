TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    utils \
    imports \
    doc \
    plugins \


android: SUBDIRS += doppelganger qt_hw_init qconnectivity
