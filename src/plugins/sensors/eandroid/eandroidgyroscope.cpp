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
#include "eandroidgyroscope.h"
#include <math.h>

EAndroidGyroscope::EAndroidGyroscope(int type, QSensor *sensor)
    : EAndroidBaseSensor(type, sensor)
{
    setReading<QGyroscopeReading>(&m_reading);
}

EAndroidGyroscope::~EAndroidGyroscope()
{
}

void EAndroidGyroscope::processEvent(sensors_event_t &event)
{
    m_reading.setTimestamp(event.timestamp / 1000);
    m_reading.setX(event.gyro.x *180/M_PI);
    m_reading.setY(event.gyro.y *180/M_PI);
    m_reading.setZ(event.gyro.z *180/M_PI);
    newReadingAvailable();
}

