/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
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
#include <eandroidaccelerometer.h>

EAndroidAccelerometer::EAndroidAccelerometer(int type, QSensor *sensor)
    : EAndroidBaseSensor(type, sensor)
{
    setReading<QAccelerometerReading>(&m_reading);
}

EAndroidAccelerometer::~EAndroidAccelerometer()
{
}

void EAndroidAccelerometer::processEvent(sensors_event_t &event)
{
    m_reading.setTimestamp(event.timestamp / 1000);
    m_reading.setX(event.data[0]);
    m_reading.setY(event.data[1]);
    m_reading.setZ(event.data[2]);
    newReadingAvailable();
}
