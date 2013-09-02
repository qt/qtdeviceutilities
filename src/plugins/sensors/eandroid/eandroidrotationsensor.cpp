#include "eandroidrotationsensor.h"
#include <math.h>

EAndroidRotationSensor::EAndroidRotationSensor(int type, QSensor *sensor)
    : EAndroidBaseSensor(type, sensor)
{
    setReading<QRotationReading>(&m_reading);
}

EAndroidRotationSensor::~EAndroidRotationSensor()
{
}

void EAndroidRotationSensor::processEvent(sensors_event_t &event)
{
    m_reading.setTimestamp(event.timestamp / 1000);

    float rz = -event.data[0] * 180 / M_PI;
    float rx = -event.data[1] * 180 / M_PI;
    float ry = event.data[2] * 180 / M_PI;

    m_reading.setFromEuler(rx, ry, rz);
    newReadingAvailable();
}
