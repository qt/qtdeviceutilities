/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSensors module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://www.qt.io/licensing.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include <qsensorplugin.h>
#include <qsensorbackend.h>
#include <qsensormanager.h>
#include <qsensor.h>

#include <eandroidsensordevice.h>

class EAndroidSensorPlugin : public QObject, public QSensorPluginInterface, public QSensorBackendFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.qt-project.Qt.QSensorPluginInterface/1.0" FILE "plugin.json")
    Q_INTERFACES(QSensorPluginInterface)
public:
    void registerSensors()
    {
#define REGISTER_BACKEND(type, id) \
        QSensorManager::registerBackend(type, id, this); \

        sensor_t const* sensors;
        QByteArray id;
        int count = EAndroidSensorDevice::instance()->availableSensors(&sensors);
        for (int i = 0; i < count; i++) {
            id = QByteArray::number(sensors[i].type);
            switch (sensors[i].type) {
            case SENSOR_TYPE_ACCELEROMETER:
                REGISTER_BACKEND(QAccelerometer::type, id)
                break;
            case SENSOR_TYPE_MAGNETIC_FIELD:
                REGISTER_BACKEND(QMagnetometer::type, id)
                break;
            case SENSOR_TYPE_ORIENTATION:
                break;
            case SENSOR_TYPE_GYROSCOPE:
                REGISTER_BACKEND(QGyroscope::type, id)
                break;
            case SENSOR_TYPE_LIGHT:
                REGISTER_BACKEND(QLightSensor::type, id);
                REGISTER_BACKEND(QAmbientLightSensor::type, id);
                break;
            case SENSOR_TYPE_PRESSURE:
                break;
            case SENSOR_TYPE_TEMPERATURE:
                break;
            case SENSOR_TYPE_PROXIMITY:
                break;
            case SENSOR_TYPE_GRAVITY:
                break;
            case SENSOR_TYPE_LINEAR_ACCELERATION:
                break;
            case SENSOR_TYPE_ROTATION_VECTOR:
                REGISTER_BACKEND(QRotationSensor::type, id);
                break;
            case SENSOR_TYPE_RELATIVE_HUMIDITY:
                break;
            case SENSOR_TYPE_AMBIENT_TEMPERATURE:
                break;
            }
        }
#undef REGISTER_BACKEND
    }

    QSensorBackend *createBackend(QSensor *sensor)
    {
        int sensorType = sensor->identifier().toInt();
        switch (sensorType) {
        case SENSOR_TYPE_ACCELEROMETER:
            return new EAndroidAccelerometer(sensorType, sensor);
        case SENSOR_TYPE_MAGNETIC_FIELD:
            return new EAndroidMagnetometer(sensorType, sensor);
        case SENSOR_TYPE_ORIENTATION:
            break;
        case SENSOR_TYPE_GYROSCOPE:
            return new EAndroidGyroscope(sensorType, sensor);
        case SENSOR_TYPE_LIGHT:
            if (sensor->type() == QAmbientLightSensor::type)
                return new EAndroidAmbientLightSensor(sensorType, sensor);
            else if (sensor->type() == QLightSensor::type)
                return new EAndroidLight(sensorType, sensor);
        case SENSOR_TYPE_PRESSURE:
            break;
        case SENSOR_TYPE_TEMPERATURE:
            break;
        case SENSOR_TYPE_PROXIMITY:
            break;
        case SENSOR_TYPE_GRAVITY:
            break;
        case SENSOR_TYPE_LINEAR_ACCELERATION:
            break;
        case SENSOR_TYPE_ROTATION_VECTOR:
            return new EAndroidRotationSensor(sensorType, sensor);
        case SENSOR_TYPE_RELATIVE_HUMIDITY:
            break;
        case SENSOR_TYPE_AMBIENT_TEMPERATURE:
            break;
        default:
            break;
        }
        return 0;
    }
};

#include "main.moc"
