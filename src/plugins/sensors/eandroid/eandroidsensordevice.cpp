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
#include <eandroidsensordevice.h>

EventReaderThread::EventReaderThread(EAndroidSensorDevice *device) :
    m_device(device)
{
}

SensorEventArray* EventReaderThread::lock()
{
    m_mutex.lock();
    return &m_events;
}

void EventReaderThread::run()
{
    static const size_t numEvents = 16;
    sensors_event_t buffer[numEvents];
    int err = 0;
    int n;
    do {
        n = m_device->m_sensorDevice->poll(m_device->m_sensorDevice, buffer, numEvents);
        if (n < 0) {
            qWarning("poll() failed (%s)\n", strerror(-err));
            break;
        }
        m_mutex.lock();
        for (int i = 0 ; i < n ; i++) {
            sensors_event_t& event = buffer[i];
            if (event.version != sizeof(sensors_event_t)) {
                qWarning("incorrect event version (version=%d, expected=%d",
                        event.version, sizeof(sensors_event_t));
                break;
            }
            m_events.append(event);
        }
        m_mutex.unlock();
        eventPending();
    } while (true);
}

EAndroidSensorDevice::EAndroidSensorDevice()
    : m_eventThread(0),
      m_sensorModule(0),
      m_sensorDevice(0),
      m_availableSensorsList(0),
      m_initSuccess(true)
{
    m_initSuccess = initSensorDevice();
    if (m_initSuccess) {
        m_eventThread = new EventReaderThread(this);
        connect(m_eventThread, SIGNAL(eventPending()), this,
            SLOT(processSensorEvents()), Qt::QueuedConnection);
    } else {
        qWarning("Failed to initialize sensor module. Possibly a missing sensor driver?");
    }
}

EAndroidSensorDevice* EAndroidSensorDevice::m_instance = 0;
EAndroidSensorDevice* EAndroidSensorDevice::instance()
{
    if (!m_instance)
        m_instance = new EAndroidSensorDevice();
    return m_instance;
}

void EAndroidSensorDevice::registerListener(int type, EAndroidBaseSensor *sensor, int dataRateHz)
{
    bool startReaderThread = m_listenersHash.isEmpty();
    bool enableSensor = m_listenersHash[type].isEmpty();
    m_listenersHash[type].push_back(sensor);
    if (startReaderThread)
        m_eventThread->start();
    if (enableSensor) {
        setActive(type, true);
        setDelay(type, dataRateHz);
    }
}

void EAndroidSensorDevice::unregisterListener(int type, EAndroidBaseSensor *sensor)
{
    m_listenersHash[type].removeOne(sensor);
    bool disableSensor = m_listenersHash[type].isEmpty();
    if (disableSensor)
        m_listenersHash.remove(type);
    bool stopReaderThread = m_listenersHash.isEmpty();
    if (stopReaderThread)
        m_eventThread->quit();
    if (disableSensor)
        setActive(type, false);
}

int EAndroidSensorDevice::indexForType(int type) const
{
    for (int i = 0; i < m_availableSensors; ++i)
        if (m_availableSensorsList[i].type == type)
            return i;
    qWarning() << "invalid sensor type: " << type;
    return -1;
}

void EAndroidSensorDevice::setActive(int type, bool enable)
{
    for (int i = 0; i < m_availableSensors; i++) {
        if (m_availableSensorsList[i].type == type) {
            int err = m_sensorDevice->activate(m_sensorDevice,
                              m_availableSensorsList[i].handle, enable);
            if (err != 0)
                qWarning("activate() for '%s'failed (%s)\n",
                        m_availableSensorsList[i].name, strerror(-err));
        }
    }
}

void EAndroidSensorDevice::setDelay(int type, int dataRateHz) const
{
    qint64 ns;
    // convert microseconds to nanoseconds
    qint32 maxRateNs = maxDataRate(type) * 1000;
    if (dataRateHz == 0) {
        // if dataRateHz is not set, then we use maxRateNs
        ns = maxRateNs;
    } else {
        // convert Hz to nanoseconds
        ns = 1000000000LL / dataRateHz;
        if (ns > maxRateNs)
            ns = maxRateNs;
    }
    int index = indexForType(type);
    if (index != -1)
        m_sensorDevice->setDelay(m_sensorDevice,
                                 m_availableSensorsList[index].handle, ns);

}

qint32 EAndroidSensorDevice::maxDataRate(int type) const
{
    // minDelay - minimum delay allowed between events in microseconds
    int index = indexForType(type);
    if (index != -1)
        return m_availableSensorsList[index].minDelay;
    return 0;
}

QString EAndroidSensorDevice::description(int type) const
{
    int index = indexForType(type);
    if (index != -1) {
        QString desc;
        desc.append(m_availableSensorsList[index].name);
        desc.append(QString(" (Vendor:%1) ").arg(m_availableSensorsList[index].vendor));
        return desc;
    }
    return QString();
}

int EAndroidSensorDevice::availableSensors(sensor_t const** list) const
{
    if (m_initSuccess)
        return m_sensorModule->get_sensors_list(m_sensorModule, list);
    return 0;
}

bool EAndroidSensorDevice::initSensorDevice()
{
    int err = 0;
    err = hw_get_module(SENSORS_HARDWARE_MODULE_ID,
                        (hw_module_t const**)&m_sensorModule);
    if (err != 0 || !m_sensorModule) {
        qWarning("hw_get_module() failed (%s)\n", strerror(-err));
        return false;
    }

    err = sensors_open(&m_sensorModule->common, &m_sensorDevice);
    if (err != 0 || !m_sensorDevice) {
        qWarning("sensors_open() failed (%s)\n", strerror(-err));
        return false;
    }

    m_availableSensors = m_sensorModule->get_sensors_list(m_sensorModule,
                                                          &m_availableSensorsList);
    return true;
}

void EAndroidSensorDevice::processSensorEvents() const
{
    SensorEventArray *eventqueue = m_eventThread->lock();
    for (int i = 0; i < eventqueue->size(); i++) {
        sensors_event_t event = eventqueue->at(i);
        foreach (EAndroidBaseSensor *sensorListener, m_listenersHash[event.type])
            sensorListener->processEvent(event);
    }
    eventqueue->clear();
    m_eventThread->unlock();
}
