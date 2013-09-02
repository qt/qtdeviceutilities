#ifndef EANDROIDACCELEROMETER_H
#define EANDROIDACCELEROMETER_H

#include <eandroidbasesensor.h>

#include <QtSensors/QAccelerometerReading>

class EAndroidAccelerometer : public EAndroidBaseSensor
{
    Q_OBJECT
public:
    EAndroidAccelerometer(int type, QSensor *sensor);
    ~EAndroidAccelerometer();
    void processEvent(sensors_event_t &event);

private:
    QAccelerometerReading m_reading;
};

#endif // EANDROIDACCELEROMETER_H
