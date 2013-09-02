#ifndef EANDROIDBASESENSOR_H
#define EANDROIDBASESENSOR_H

#include <qsensorbackend.h>
#include <hardware/sensors.h>

class EAndroidBaseSensor : public QSensorBackend
{
    Q_OBJECT
public:
    EAndroidBaseSensor(int sensorType, QSensor *sensor);
    virtual ~EAndroidBaseSensor();
    virtual void start();
    virtual void stop();
    virtual void processEvent(sensors_event_t &event);

private:
    int m_type;
    bool m_isStarted;
};

#endif // EANDROIDBASESENSOR_H
