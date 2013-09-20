TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    utils \
    imports \
    doc \
    plugins \


android: SUBDIRS += doppelganger
