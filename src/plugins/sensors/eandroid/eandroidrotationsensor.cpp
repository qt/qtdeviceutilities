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
#include "eandroidrotationsensor.h"
#include <math.h>

EAndroidRotationSensor::EAndroidRotationSensor(int type, QSensor *sensor)
    : EAndroidBaseSensor(type, sensor)
{
    setReading<QRotationReading>(&m_reading);
}

EAndroidRotationSensor::~EAndroidRotationSensor()
{
}

void EAndroidRotationSensor::processEvent(sensors_event_t &event)
{
    m_reading.setTimestamp(event.timestamp / 1000);

    float rz = -event.data[0] * 180 / M_PI;
    float rx = -event.data[1] * 180 / M_PI;
    float ry = event.data[2] * 180 / M_PI;

    m_reading.setFromEuler(rx, ry, rz);
    newReadingAvailable();
}
