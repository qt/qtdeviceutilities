#include "qdroidutils.h"
#include <unistd.h>
#include <QDebug>
#include <math.h>

#ifdef Q_OS_ANDROID_NO_SDK
#include <cutils/android_reboot.h>
#include <hardware/lights.h>
#include <media/AudioSystem.h>
#else
#include <sys/reboot.h>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QFile>
#endif

/*!
 * Reboots the system. Does not return.
 *
 * \sa powerOffSystem()
 */
void QDroidUtils::rebootSystem()
{
    sync();
#ifdef Q_OS_ANDROID_NO_SDK
    (void)android_reboot(ANDROID_RB_RESTART, 0, 0);
#else
    reboot(RB_AUTOBOOT);
#endif
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
#ifdef Q_OS_ANDROID_NO_SDK
    (void)android_reboot(ANDROID_RB_POWEROFF, 0, 0);
#else
    reboot(RB_POWER_OFF);
#endif
    qWarning("powerOff returned");
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
 * Gets the current IP address of the device
 */
QString QDroidUtils::getIPAddress()
{
    QString address;
#ifdef Q_OS_ANDROID_NO_SDK
    qDebug("QDroidUtils::getIPAddress()");
#else
    QNetworkInterface interface = QNetworkInterface::interfaceFromName("eth0");
    QList<QNetworkAddressEntry> entries;
    entries = interface.addressEntries();
    if ( !entries.empty() ) {
        address = entries.first().ip().toString();
    }
#endif
    return address;
}

/*!
 * Gets the current hostname of the device
 */
QString QDroidUtils::getHostname()
{
    QString hostname;
#ifdef Q_OS_ANDROID_NO_SDK
    qDebug("QDroidUtils::getHostname()");
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
    qDebug("QDroidUtils::setHostname()");
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
