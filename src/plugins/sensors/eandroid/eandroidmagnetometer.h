#ifndef EANDROIDMAGNETOMETER_H
#define EANDROIDMAGNETOMETER_H

#include <eandroidbasesensor.h>

#include <QtSensors/QMagnetometerReading>

class EAndroidMagnetometer : public EAndroidBaseSensor
{
    Q_OBJECT
public:
    EAndroidMagnetometer(int type, QSensor *sensor);
    ~EAndroidMagnetometer();
    void processEvent(sensors_event_t &event);

private:
    QMagnetometerReading m_reading;
};

#endif // EANDROIDMAGNETOMETER_H
