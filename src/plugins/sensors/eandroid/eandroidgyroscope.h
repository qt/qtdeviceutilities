#ifndef EANDROIDGYROSCOPE_H
#define EANDROIDGYROSCOPE_H

#include <eandroidbasesensor.h>

#include <QtSensors/QGyroscopeReading>

class EAndroidGyroscope : public EAndroidBaseSensor
{
    Q_OBJECT
public:
    EAndroidGyroscope(int type, QSensor *sensor);
    ~EAndroidGyroscope();
    void processEvent(sensors_event_t &event);

private:
    QGyroscopeReading m_reading;
};

#endif // EANDROIDGYROSCOPE_H
