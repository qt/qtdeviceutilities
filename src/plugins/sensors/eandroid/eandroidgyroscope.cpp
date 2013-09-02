#include "eandroidgyroscope.h"
#include <math.h>

EAndroidGyroscope::EAndroidGyroscope(int type, QSensor *sensor)
    : EAndroidBaseSensor(type, sensor)
{
    setReading<QGyroscopeReading>(&m_reading);
}

EAndroidGyroscope::~EAndroidGyroscope()
{
}

void EAndroidGyroscope::processEvent(sensors_event_t &event)
{
    m_reading.setTimestamp(event.timestamp / 1000);
    m_reading.setX(event.gyro.x *180/M_PI);
    m_reading.setY(event.gyro.y *180/M_PI);
    m_reading.setZ(event.gyro.z *180/M_PI);
    newReadingAvailable();
}

