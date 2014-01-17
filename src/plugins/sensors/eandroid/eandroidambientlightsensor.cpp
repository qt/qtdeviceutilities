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
#include "eandroidambientlightsensor.h"

EAndroidAmbientLightSensor::EAndroidAmbientLightSensor(int type, QSensor *sensor)
    : EAndroidBaseSensor(type, sensor)
{
    setReading<QAmbientLightReading>(&m_reading);
}

EAndroidAmbientLightSensor::~EAndroidAmbientLightSensor()
{
}

void EAndroidAmbientLightSensor::processEvent(sensors_event_t &event)
{
    const int lightLevelLux = event.light;

    if (lightLevelLux < 10)
        m_reading.setLightLevel(QAmbientLightReading::Dark);
    else if (lightLevelLux < 80)
        m_reading.setLightLevel(QAmbientLightReading::Twilight);
    else if (lightLevelLux < 400)
        m_reading.setLightLevel(QAmbientLightReading::Light);
    else if (lightLevelLux < 2500)
        m_reading.setLightLevel(QAmbientLightReading::Bright);
    else
        m_reading.setLightLevel(QAmbientLightReading::Sunny);

    newReadingAvailable();
}
