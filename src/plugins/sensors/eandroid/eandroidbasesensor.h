/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://www.qt.io
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://www.qt.io
**
****************************************************************************/
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
