#ifndef EANDROIDLIGHT_H
#define EANDROIDLIGHT_H

#include <eandroidbasesensor.h>

#include <QtSensors/QLightReading>

class EAndroidLight : public EAndroidBaseSensor
{
    Q_OBJECT
public:
    EAndroidLight(int type, QSensor *sensor);
    ~EAndroidLight();
    void processEvent(sensors_event_t &ev);

private:
    QLightReading m_reading;
};

#endif // EANDROIDLIGHT_H
