TARGET = qtsensors_eandroid
QT = core sensors

PLUGIN_TYPE = sensors
load(qt_plugin)

LIBS += -lhardware

HEADERS += \
    eandroidambientlightsensor.h \
    eandroidsensordevice.h \
    eandroidbasesensor.h \
    eandroidaccelerometer.h \
    eandroidlight.h \
    eandroidgyroscope.h \
    eandroidmagnetometer.h \
    eandroidrotationsensor.h

SOURCES += \
    main.cpp \
    eandroidambientlightsensor.cpp \
    eandroidsensordevice.cpp \
    eandroidbasesensor.cpp \
    eandroidaccelerometer.cpp \
    eandroidlight.cpp \
    eandroidgyroscope.cpp \
    eandroidmagnetometer.cpp \
    eandroidrotationsensor.cpp

OTHER_FILES = plugin.json
