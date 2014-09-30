/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://www.qt.io
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://www.qt.io
**
****************************************************************************/
#include "qdroidutils.h"
#include <unistd.h>
#include <QDebug>
#include <math.h>
#include <sys/reboot.h>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QFile>

#ifdef Q_OS_ANDROID_NO_SDK
#include <cutils/properties.h>
#include <hardware/lights.h>
#include <media/AudioSystem.h>
#include <utils/String8.h>
#endif

/*!
 * Reboots the system. Does not return.
 *
 * \sa powerOffSystem()
 */
void QDroidUtils::rebootSystem()
{
    sync();
    reboot(RB_AUTOBOOT);
    qWarning("reboot returned");
}

/*!
 * Shuts down the system. Does not return.
 *
 * \sa rebootSystem()
 */
void QDroidUtils::powerOffSystem()
{
    sync();
    reboot(RB_POWER_OFF);
    qWarning("powerOff returned");
}

void QDroidUtils::setOrientationForAudioSystem(AudioOrientation orientation)
{
#ifdef Q_OS_ANDROID_NO_SDK
    QString orientationString = QStringLiteral("undefined");
    switch (orientation) {
    case LandscapeAudioOrientation:
        orientationString = QStringLiteral("landscape");
        break;
    case PortraitAudioOrientation:
        orientationString = QStringLiteral("portrait");
        break;
    case SquareAudioOrientation:
        orientationString = QStringLiteral("square");
        break;
    default:
        break;
    }
    android::AudioSystem::setParameters(0, android::String8(QStringLiteral("orientation=%2")
                                                            .arg(orientationString).toLatin1().constData()));
#endif
}

/*!
 * Sets the master volume to \a volume.
 * The volume can range from 0 to 100 and is linear.
 * Changing the master volume will affect all audio streams.
 *
 * \sa setStreamVolume()
 * \sa setMasterMute()
 */
void QDroidUtils::setMasterVolume(int volume)
{
#ifdef Q_OS_ANDROID_NO_SDK
    android::status_t rc;
    volume = qBound(0, volume, 100);
    rc = android::AudioSystem::setMasterVolume(android::AudioSystem::linearToLog(volume));
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while setting audio properties.";
#endif
}

/*!
 * Sets the master mute to \a mute. Setting it to true will disable all
 * sounds on the device.
 *
 * \sa setMasterVolume()
 * \sa setStreamMute()
 */
void QDroidUtils::setMasterMute(bool mute)
{
#ifdef Q_OS_ANDROID_NO_SDK
    android::status_t rc;
    rc = android::AudioSystem::setMasterMute(mute);
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while setting audio properties.";
#endif
}

/*!
    \enum QDroidUtils::AudioStreamType
    \value DefaultAudioStream
           The default audio stream

    \value VoiceCallAudioStream
           The audio stream for phone calls

    \value SystemAudioStream
           The audio stream for system sounds

    \value RingAudioStream
           The audio stream for the phone ring

    \value AlarmAudioStream
           The audio stream for alarms

    \value NotificationAudioStream
           The audio stream for notifications

    \value BluetoothAudioStream
           The audio stream for audio transmitted over bluetooth

    \value EnforcedAudibleAudioStream
           Sounds that cannot be muted by user and must be routed to speaker

    \value DTMFAudioStream
           The audio stream for DTMF Tones

    \value TTSAudioStream
           The audio stream for text-to-speech
*/

/*!
 * Sets the volume for a specific audio \a stream type to \a volume.
 * The volume can range from 0 to 100 and is linear.
 * All streams of the specified type will be affected.
 *
 * \sa setMasterVolume()
 * \sa setStreamMute()
 */
void QDroidUtils::setStreamVolume(AudioStreamType streamType, int volume)
{
#ifdef Q_OS_ANDROID_NO_SDK
    android::status_t rc;
    volume = qBound(0, volume, 100);
    rc = android::AudioSystem::setStreamVolume(audio_stream_type_t(streamType),
                                          android::AudioSystem::linearToLog(volume), 0);
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while setting audio properties.";
#endif
}

/*!
 * Mutes all audio \a streams of type \a streamType.
 *
 * \sa setStreamVolume()
 * \sa setMasterMute()
 */
void QDroidUtils::setStreamMute(AudioStreamType streamType, bool mute)
{
#ifdef Q_OS_ANDROID_NO_SDK
    android::status_t rc;
    rc = android::AudioSystem::setStreamMute(audio_stream_type_t(streamType), mute);
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while setting audio properties.";
#endif
}

/*!
 * Sets the display brightness (i.e. the intensity of the backlight)
 * to \a value. A value of 255 requests maximum brightness, while 0 requests
 * minimum (typically, the backlight turned off).
 *
 * Returns true on success.
 */
//### TBD: add the user/sensor setting as parameter!
bool QDroidUtils::setDisplayBrightness(quint8 value)
{
#ifdef Q_OS_ANDROID_NO_SDK
    const struct hw_module_t* module = 0;
    if (hw_get_module(LIGHTS_HARDWARE_MODULE_ID, &module))
        return false;
    if (!module || !module->methods || !module->methods->open)
        return false;

    struct light_device_t* device = 0;
    if (module->methods->open(module, LIGHT_ID_BACKLIGHT, (struct hw_device_t**)&device))
        return false;
    if (!device || !device->set_light || !device->common.close)
        return false;

    struct light_state_t state;
    memset(&state, 0, sizeof(light_state_t));
    state.color = 0xff000000 | (value << 16) | (value << 8) | value;
    if (!device->set_light(device, &state))
        return false;

    device->common.close(&device->common);
#else
    qDebug("QDroidUtils::setDisplayBrightness(%i)", value);
#endif
    return true;
}


/*!
 * Gets the current IP address(es) of the device
 */
QString QDroidUtils::getIPAddress()
{
    QList<QNetworkInterface> availableInterfaces = QNetworkInterface::allInterfaces();
    if (availableInterfaces.length() > 0) {
        foreach (const QNetworkInterface &interface, availableInterfaces) {
            if (interface.flags() & QNetworkInterface::IsRunning
                && (interface.flags() & QNetworkInterface::IsLoopBack) == 0) {
                QList<QNetworkAddressEntry> entries = interface.addressEntries();
                QStringList addresses;
                foreach (const QNetworkAddressEntry &entry, entries)
                    addresses.append(entry.ip().toString().split('%').first());
                return addresses.join(QStringLiteral(", "));
            }
        }
    }
    return QString();
}

/*!
 * Gets the current hostname of the device
 */
QString QDroidUtils::getHostname()
{
    QString hostname;
#ifdef Q_OS_ANDROID_NO_SDK
    char prop_value[PROPERTY_VALUE_MAX];
    int len = property_get("net.hostname", prop_value, 0);
    if (len)
        hostname = QString::fromLocal8Bit(prop_value, len);
#else
    hostname = QHostInfo::localHostName();
#endif
    return hostname;
}

/*!
 * Sets new hostname for the device
 */
bool QDroidUtils::setHostname(QString hostname)
{
#ifdef Q_OS_ANDROID_NO_SDK
    property_set("net.hostname", hostname.toLocal8Bit().constData());
#else
    QFile file("/etc/hostname");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Could not open hostname file");
        return false;
    }
    file.write(hostname.toLocal8Bit());
    file.close();
#endif
    return true;
}

float QDroidUtils::masterVolume() const
{
    float volume = NAN;
#ifdef Q_OS_ANDROID_NO_SDK
    android::status_t rc;
    rc = android::AudioSystem::getMasterVolume(&volume);
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while getting audio properties.";
#endif
    return volume;
}

bool QDroidUtils::masterMute() const
{
    bool mute = false;
#ifdef Q_OS_ANDROID_NO_SDK
    android::status_t rc;
    rc = android::AudioSystem::getMasterMute(&mute);
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while getting audio properties.";
#endif
    return mute;
}

float QDroidUtils::streamVolume(AudioStreamType stream) const
{
    float volume = NAN;
#ifdef Q_OS_ANDROID_NO_SDK
    android::status_t rc;
    rc = android::AudioSystem::getStreamVolume(audio_stream_type_t(stream), &volume, 0);
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while getting audio properties.";
#endif
    return volume;
}

bool QDroidUtils::streamMute(AudioStreamType stream) const
{
    bool mute = false;
#ifdef Q_OS_ANDROID_NO_SDK
    android::status_t rc;
    rc = android::AudioSystem::getStreamMute(audio_stream_type_t(stream), &mute);
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while getting audio properties.";
#endif
    return mute;
}
