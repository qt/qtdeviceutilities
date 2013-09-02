#ifndef EANDROIDAMBIENTLIGHTSENSOR_H
#define EANDROIDAMBIENTLIGHTSENSOR_H

#include <eandroidbasesensor.h>

#include <QtSensors/QAmbientLightReading>

class EAndroidAmbientLightSensor : public EAndroidBaseSensor
{
    Q_OBJECT
public:
    EAndroidAmbientLightSensor(int type, QSensor *sensor);
    ~EAndroidAmbientLightSensor();
    void processEvent(sensors_event_t &event);

private:
    QAmbientLightReading m_reading;
};

#endif // EANDROIDAMBIENTLIGHTSENSOR_H
