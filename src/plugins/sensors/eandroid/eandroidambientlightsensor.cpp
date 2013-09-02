#include "eandroidambientlightsensor.h"

EAndroidAmbientLightSensor::EAndroidAmbientLightSensor(int type, QSensor *sensor)
    : EAndroidBaseSensor(type, sensor)
{
    setReading<QAmbientLightReading>(&m_reading);
}

EAndroidAmbientLightSensor::~EAndroidAmbientLightSensor()
{
}

void EAndroidAmbientLightSensor::processEvent(sensors_event_t &event)
{
    const int lightLevelLux = event.light;

    if (lightLevelLux < 10)
        m_reading.setLightLevel(QAmbientLightReading::Dark);
    else if (lightLevelLux < 80)
        m_reading.setLightLevel(QAmbientLightReading::Twilight);
    else if (lightLevelLux < 400)
        m_reading.setLightLevel(QAmbientLightReading::Light);
    else if (lightLevelLux < 2500)
        m_reading.setLightLevel(QAmbientLightReading::Bright);
    else
        m_reading.setLightLevel(QAmbientLightReading::Sunny);

    newReadingAvailable();
}
