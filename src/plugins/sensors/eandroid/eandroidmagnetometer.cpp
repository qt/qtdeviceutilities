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
#include "eandroidmagnetometer.h"

EAndroidMagnetometer::EAndroidMagnetometer(int type, QSensor *sensor)
    : EAndroidBaseSensor(type, sensor)
{
    setReading<QMagnetometerReading>(&m_reading);
}

EAndroidMagnetometer::~EAndroidMagnetometer()
{
}

void EAndroidMagnetometer::processEvent(sensors_event_t &event)
{
    m_reading.setTimestamp(event.timestamp / 1000);
    // convect micro-Tesla to tesla
    m_reading.setX(event.magnetic.x / 1e6);
    m_reading.setY(event.magnetic.y / 1e6);
    m_reading.setZ(event.magnetic.z / 1e6);

    switch (event.magnetic.status) {
    case SENSOR_STATUS_UNRELIABLE:
        m_reading.setCalibrationLevel(0.0);
        break;
    case SENSOR_STATUS_ACCURACY_LOW:
        m_reading.setCalibrationLevel(0.3);
        break;
    case SENSOR_STATUS_ACCURACY_MEDIUM:
        m_reading.setCalibrationLevel(0.6);
        break;
    case SENSOR_STATUS_ACCURACY_HIGH:
        m_reading.setCalibrationLevel(1.0);
        break;
    default:
        break;
    }

    newReadingAvailable();
}
