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
#ifndef EANDROIDSENSORDEVICE_H
#define EANDROIDSENSORDEVICE_H

#include <eandroidaccelerometer.h>
#include <eandroidlight.h>
#include <eandroidambientlightsensor.h>
#include <eandroidgyroscope.h>
#include <eandroidmagnetometer.h>
#include <eandroidrotationsensor.h>

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QReadWriteLock>
#include <QtCore/QVarLengthArray>
#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QDebug>

typedef QVarLengthArray<sensors_event_t, 16> SensorEventArray;
typedef QHash<int, QList<EAndroidBaseSensor *> > ListenersHash;

class EAndroidSensorDevice;

class EventReaderThread : public QThread
{
    Q_OBJECT
public:
    EventReaderThread(EAndroidSensorDevice *manager);
    void run();
    SensorEventArray* lock();
    inline void unlock() { m_mutex.unlock(); }

signals:
    void eventPending();

private:
    EAndroidSensorDevice *m_device;
    SensorEventArray m_events;
    QMutex m_mutex;
};

class EAndroidSensorDevice : public QObject
{
    Q_OBJECT
public:
    static EAndroidSensorDevice* instance();

    void registerListener(int type, EAndroidBaseSensor *sensor, int dataRateHz);
    void unregisterListener(int type, EAndroidBaseSensor *sensor);
    int availableSensors(sensor_t const** list) const;
    qint32 maxDataRate(int type) const;
    QString description(int type) const;

protected:
    bool initSensorDevice();
    int indexForType(int type) const;
    void setActive(int type, bool enable);
    void setDelay(int type, int dataRateHz) const;

public slots:
    void processSensorEvents() const;

private:
    static EAndroidSensorDevice* m_instance;
    EAndroidSensorDevice();
    EAndroidSensorDevice(const EAndroidSensorDevice &);
    EAndroidSensorDevice& operator=(const EAndroidSensorDevice &);

    friend class EventReaderThread;
    EventReaderThread *m_eventThread;

    sensors_module_t* m_sensorModule;
    sensors_poll_device_t* m_sensorDevice;
    sensor_t const* m_availableSensorsList;
    int m_availableSensors;
    bool m_initSuccess;

    ListenersHash m_listenersHash;
};

#endif // EANDROIDSENSORDEVICE_H
