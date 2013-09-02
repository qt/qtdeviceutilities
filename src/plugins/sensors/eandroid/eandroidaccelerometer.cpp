#include <eandroidaccelerometer.h>

EAndroidAccelerometer::EAndroidAccelerometer(int type, QSensor *sensor)
    : EAndroidBaseSensor(type, sensor)
{
    setReading<QAccelerometerReading>(&m_reading);
}

EAndroidAccelerometer::~EAndroidAccelerometer()
{
}

void EAndroidAccelerometer::processEvent(sensors_event_t &event)
{
    m_reading.setTimestamp(event.timestamp / 1000);
    m_reading.setX(event.data[0]);
    m_reading.setY(event.data[1]);
    m_reading.setZ(event.data[2]);
    newReadingAvailable();
}
