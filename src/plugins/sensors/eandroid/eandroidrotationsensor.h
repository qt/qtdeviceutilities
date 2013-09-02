#ifndef EANDROIDROTATIONSENSOR_H
#define EANDROIDROTATIONSENSOR_H

#include <eandroidbasesensor.h>

#include <QtSensors/QRotationReading>

class EAndroidRotationSensor : public EAndroidBaseSensor
{
    Q_OBJECT
public:
    EAndroidRotationSensor(int type, QSensor *sensor);
    ~EAndroidRotationSensor();
    void processEvent(sensors_event_t &event);

private:
    QRotationReading m_reading;
};

#endif // EANDROIDROTATIONSENSOR_H
