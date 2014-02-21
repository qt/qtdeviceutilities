/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://qt.digia.com/
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://qt.digia.com/
**
****************************************************************************/
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
