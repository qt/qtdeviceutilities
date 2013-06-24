TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    utils \
    imports \
    doc

android: SUBDIRS += doppelganger
