#include <eandroidlight.h>

EAndroidLight::EAndroidLight(int type, QSensor *sensor)
    : EAndroidBaseSensor(type, sensor)
{
    setReading<QLightReading>(&m_reading);
}

EAndroidLight::~EAndroidLight()
{
}

void EAndroidLight::processEvent(sensors_event_t &event)
{
    m_reading.setTimestamp(event.timestamp / 1000);
    m_reading.setLux(event.light);
    newReadingAvailable();
}
