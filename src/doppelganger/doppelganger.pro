QT -= core gui

TARGET = doppelganger

LIBS += -lutils -lbinder -lcutils \
        -L$${ANDROID_PRODUCT_OUT}/obj/STATIC_LIBRARIES/libscheduling_policy_intermediates -lscheduling_policy -lpowermanager

TEMPLATE = app

SOURCES += main.cpp \
    permissioncontroller.cpp \
    schedulingpolicyservice.cpp \
    powermanager.cpp

HEADERS += \
    permissioncontroller.h \
    schedulingpolicyservice.h \
    powermanager.h

load(qt_tool)
