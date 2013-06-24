QT -= core gui

TARGET = doppelganger

LIBS += -lutils -lbinder -lcutils \
        -L$${ANDROID_PRODUCT_OUT}/obj/STATIC_LIBRARIES/libscheduling_policy_intermediates -lscheduling_policy

TEMPLATE = app

SOURCES += main.cpp \
    permissioncontroller.cpp \
    schedulingpolicyservice.cpp

HEADERS += \
    permissioncontroller.h \
    schedulingpolicyservice.h

load(qt_tool)
