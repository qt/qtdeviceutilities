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
