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
#include "b2qtdevice.h"
#include <unistd.h>
#include <QDebug>
#include <math.h>
#include <sys/reboot.h>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QFile>
#include <QDirIterator>
#include <QTimer>
#include <QtCore/qmath.h>

#ifdef Q_OS_ANDROID_NO_SDK
#include <cutils/properties.h>
#include <hardware/lights.h>
#include <media/AudioSystem.h>
#include <utils/String8.h>
#endif

// When we can't query directly, at least remember what we have set it to
static quint8 knownBrightness = 255;

B2QtDevice::B2QtDevice(QObject *parent)
    : QObject(parent)
{
}

B2QtDevice::~B2QtDevice()
{
}

/*!
 * Reboots the system. Does not return.
 *
 * \sa powerOff()
 */
void B2QtDevice::reboot()
{
    sync();
    ::reboot(RB_AUTOBOOT);
    qWarning("reboot returned");
}


/*!
 * Shuts down the system. Does not return.
 *
 * \sa reboot()
 */
void B2QtDevice::powerOff()
{
    sync();
    ::reboot(RB_POWER_OFF);
    qWarning("powerOff returned");
}


class LightDevice
{
public:
    QString name;
    QString deviceFile;
    quint8 value;
    uint maxValue;
};

static QList<LightDevice> lightDevices;
static bool lightDevicesInitialized = false;

static void initLightDevices()
{
    if (lightDevicesInitialized)
        return;
    QDirIterator it(QStringLiteral("/sys/class/backlight"));
    while (it.hasNext()) {
        LightDevice ld;
        ld.deviceFile = it.next() + QStringLiteral("/brightness");
        QFile maxFile(it.filePath() + QStringLiteral("/max_brightness"));
        if (!maxFile.open(QIODevice::ReadOnly))
            continue;
        bool ok = false;
        ld.maxValue = maxFile.read(10).simplified().toUInt(&ok);
        if (!ok || !ld.maxValue)
            continue;
        QFile valFile(ld.deviceFile);
        if (!valFile.open(QIODevice::ReadOnly))
            continue;
        ok = false;
        uint val = valFile.read(10).simplified().toUInt(&ok);
        if (!ok)
            continue;
        // map max->max as that is a common case, otherwise choose a reasonable value
        ld.value = (val == ld.maxValue) ? 255 : (val * 256)/(ld.maxValue+1);
        ld.name = it.fileName();
        lightDevices.append(ld);
    }
    if (!lightDevices.isEmpty())
        knownBrightness = lightDevices.at(0).value;
    lightDevicesInitialized = true;
}

/*!
 * Sets the display brightness (i.e. the intensity of the backlight)
 * to \a value. A value of 255 requests maximum brightness, while 0 requests
 * minimum (typically, the backlight turned off).
 *
 * Returns true on success.
 */
bool B2QtDevice::setDisplayBrightness(int v)
{
    quint8 value = qBound(0, v, 255);
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
    initLightDevices();
    for (int i = 0; i < lightDevices.size(); i++) {
        LightDevice &ld = lightDevices[i];
        QFile devFile(ld.deviceFile);
        if (!devFile.open(QIODevice::WriteOnly))
            continue;
        // Maps only 0 to 0, since 0 often means "off"; other values are degrees of "on".
        uint newVal = value ? 1 + ((value * ld.maxValue) / 256) : 0;
        devFile.write(QByteArray::number(newVal));
        ld.value = value;
    }
#endif
    knownBrightness = value;
    return true;
}


/*!
 * Returns the current backlight intensity.
 * \sa setDisplayBrightness
 */
int B2QtDevice::displayBrightness() const
{
#ifdef Q_OS_ANDROID_NO_SDK
    QFile sysFile(QStringLiteral("/sys/class/leds/lcd-backlight/brightness"));
    if (sysFile.open(QIODevice::ReadOnly | QIODevice::Unbuffered)) {
        bool ok = false;
        int sysVal = sysFile.read(3).simplified().toInt(&ok);
        if (ok)
            knownBrightness = qBound(0, sysVal, 255);
    }
#else
    initLightDevices();
#endif

    return knownBrightness;
}


/*!
 * Gets the current IP address(es) of the device
 */
QString B2QtDevice::getIPAddress() const
{
    QStringList addresses;
    foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces()) {
        QNetworkInterface::InterfaceFlags flags = interface.flags();
        if (flags.testFlag(QNetworkInterface::IsRunning) && !flags.testFlag(QNetworkInterface::IsLoopBack)) {
            foreach (const QNetworkAddressEntry &entry, interface.addressEntries())
                addresses.append(entry.ip().toString().split('%').first());
        }
    }
    return addresses.join(QStringLiteral(", "));
}


/*!
 * Gets the current hostname of the device
 */
QString B2QtDevice::hostname() const
{
    QString name;
#ifdef Q_OS_ANDROID_NO_SDK
    char prop_value[PROPERTY_VALUE_MAX];
    int len = property_get("net.hostname", prop_value, 0);
    if (len)
        name = QString::fromLocal8Bit(prop_value, len);
#else
    name = QHostInfo::localHostName();
#endif
    return name;
}


/*!
 * Sets new hostname for the device
 */
bool B2QtDevice::setHostname(const QString &name)
{
#ifdef Q_OS_ANDROID_NO_SDK
    property_set("net.hostname", name.toLocal8Bit().constData());
#else
    QByteArray lname = name.toLocal8Bit();
    if (::sethostname(lname.constData(), lname.length())) {
        qWarning("Could not set system hostname");
        return false;
    }
    // Also store it for next boot:
    QFile file(QStringLiteral("/etc/hostname"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning("Could not write to /etc/hostname");
        return false;
    }
    file.write(lname.append('\n'));
    file.close();
#endif
    emit hostnameChanged(name);
    return true;
}


/*!
 * Sets the master volume to \a volume.
 * The volume can range from 0 to 100 and is linear.
 */
void B2QtDevice::setMasterVolume(int volume)
{
#ifdef Q_OS_ANDROID_NO_SDK
    android::status_t rc;
    volume = qBound(0, volume, 100);
    rc = android::AudioSystem::setMasterVolume(android::AudioSystem::linearToLog(volume));
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while setting audio properties.";
    else
        emit masterVolumeChanged(volume);
#else
    Q_UNUSED(volume)
#endif
}


/*!
 * Returns the current master volume.
 * The volume can range from 0 to 100 and is linear.
 */
int B2QtDevice::masterVolume() const
{
    float volume = 0;
#ifdef Q_OS_ANDROID_NO_SDK
    android::status_t rc;
    rc = android::AudioSystem::getMasterVolume(&volume);
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while getting audio properties.";
#endif
    return qBound(0, qRound(volume), 100);
}

#ifdef Q_OS_ANDROID_NO_SDK
// Android audio handling

enum AudioOrientation {
    LandscapeAudioOrientation,
    PortraitAudioOrientation,
    SquareAudioOrientation,
    UndefinedAudioOrientation,
};

enum AudioStreamType {
    DefaultAudioStream = -1,
    VoiceCallAudioStream = 0,
    SystemAudioStream = 1,
    RingAudioStream = 2,
    MusicAudioStream = 3,
    AlarmAudioStream = 4,
    NotificationAudioStream = 5,
    BluetoothAudioStream    = 6,
    EnforcedAudibleAudioStream = 7,
    DTMFAudioStream = 8,
    TTSAudioStream = 9
};


/*
    \enum AudioStreamType
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

/*
 * Sets the volume for a specific audio \a stream type to \a volume.
 * The volume can range from 0 to 100 and is linear.
 * All streams of the specified type will be affected.
 *
 * \sa setMasterVolume()
 * \sa setStreamMute()
 */
void setStreamVolume(AudioStreamType streamType, int volume)
{
    android::status_t rc;
    volume = qBound(0, volume, 100);
    rc = android::AudioSystem::setStreamVolume(audio_stream_type_t(streamType),
                                          android::AudioSystem::linearToLog(volume), 0);
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while setting audio properties.";
}

/*
 * Mutes all audio \a streams of type \a streamType.
 *
 * \sa setStreamVolume()
 * \sa setMasterMute()
 */
void setStreamMute(AudioStreamType streamType, bool mute)
{
    android::status_t rc;
    rc = android::AudioSystem::setStreamMute(audio_stream_type_t(streamType), mute);
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while setting audio properties.";
}

void setOrientationForAudioSystem(AudioOrientation orientation)
{
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
}


/*!
 * Sets the master mute to \a mute. Setting it to true will disable all
 * sounds on the device.
 *
 * \sa setMasterVolume()
 * \sa setStreamMute()
 */
void setMasterMute(bool mute)
{

    android::status_t rc;
    rc = android::AudioSystem::setMasterMute(mute);
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while setting audio properties.";
}

bool masterMute()
{
    bool mute = false;
    android::status_t rc;
    rc = android::AudioSystem::getMasterMute(&mute);
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while getting audio properties.";
    return mute;
}

float streamVolume(AudioStreamType stream)
{
    float volume = NAN;
    android::status_t rc;
    rc = android::AudioSystem::getStreamVolume(audio_stream_type_t(stream), &volume, 0);
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while getting audio properties.";
    return volume;
}

bool streamMute(AudioStreamType stream)
{
    bool mute = false;
    android::status_t rc;
    rc = android::AudioSystem::getStreamMute(audio_stream_type_t(stream), &mute);
    if (rc != android::NO_ERROR)
        qWarning() << Q_FUNC_INFO << "Error while getting audio properties.";
    return mute;
}

#endif

/*!
 * Initializes the audio subsystem, setting the volume to max.
 * This is done during system startup, so there is normally no need to call this function from applications.
 */
void B2QtDevice::initAudio()
{
#ifdef Q_OS_ANDROID_NO_SDK
    // Set the audio orientation to something to force the HW driver to reconfigure
    // audio routing (workaround for bug on Nexus 7)
    setOrientationForAudioSystem(LandscapeAudioOrientation);
    setMasterVolume(100);
    setMasterMute(false);
    setStreamVolume(SystemAudioStream, 100);
    setStreamVolume(MusicAudioStream, 100);
    setStreamVolume(NotificationAudioStream, 100);
    setStreamVolume(EnforcedAudibleAudioStream, 100);
#endif
}

class PhysicalScreenSize : public QObject
{
    Q_OBJECT

public:
    PhysicalScreenSize();

    void setSize(int inches);
    int size() const { return physScreenSizeInch; }
    bool enabled() const;
    void setEnabled(bool enable);

private slots:
    void onTimeout();

private:
    void read(const QString &filename);
    void write(bool includePhysSize = true);
    void write(const QString &filename, bool includePhysSize = true);

    bool physScreenSizeEnabled;
    int physScreenSizeInch;
    QTimer physWriteTimer;
};

Q_GLOBAL_STATIC(PhysicalScreenSize, physScreenSize)

PhysicalScreenSize::PhysicalScreenSize()
    : physScreenSizeEnabled(false), physScreenSizeInch(7)
{
    physWriteTimer.setSingleShot(true);
    physWriteTimer.setInterval(1000);
    QObject::connect(&physWriteTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    read(QStringLiteral("/etc/appcontroller.conf"));
    read(QStringLiteral("/var/lib/b2qt/appcontroller.conf.d/physical_screen_size.conf"));
}

void PhysicalScreenSize::read(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    int physScreenWidth = 154, physScreenHeight = 90;
    int found = 0;
    while (!f.atEnd()) {
        QByteArray line = f.readLine().trimmed();
        if (line.startsWith(QByteArrayLiteral("env="))) {
            QByteArrayList values = line.split('=');
            if (values.count() == 3) {
                bool ok;
                if (values[1] == QByteArrayLiteral("QT_QPA_EGLFS_PHYSICAL_WIDTH")) {
                    int val = values[2].toInt(&ok);
                    if (ok) {
                        ++found;
                        physScreenWidth = val;
                    }
                } else if (values[1] == QByteArrayLiteral("QT_QPA_EGLFS_PHYSICAL_HEIGHT")) {
                    int val = values[2].toInt(&ok);
                    if (ok) {
                        ++found;
                        physScreenHeight = val;
                    }
                }
            }
        }
    }
    if (found == 2)
        physScreenSizeEnabled = true;

    const qreal diagMM = qSqrt(physScreenWidth * physScreenWidth + physScreenHeight * physScreenHeight);
    physScreenSizeInch = qRound(diagMM / 25.4);
}

void PhysicalScreenSize::onTimeout()
{
    write();
}

void PhysicalScreenSize::write(bool includePhysSize)
{
    QDir(QStringLiteral("/var/lib")).mkpath(QStringLiteral("b2qt/appcontroller.conf.d"));
    write(QStringLiteral("/var/lib/b2qt/appcontroller.conf.d/physical_screen_size.conf"), includePhysSize);
}

void PhysicalScreenSize::write(const QString &filename, bool includePhysSize)
{
    QFile f(filename);

    QByteArrayList lines;
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!f.atEnd()) {
            QByteArray line = f.readLine().trimmed();
            if (!line.startsWith(QByteArrayLiteral("env=QT_QPA_EGLFS_PHYSICAL_WIDTH="))
                && !line.startsWith(QByteArrayLiteral("env=QT_QPA_EGLFS_PHYSICAL_HEIGHT=")))
                lines.append(line);
        }
        f.close();
    }

    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return;

    const qreal diagMM = physScreenSizeInch * 25.4;
    // Assume 16:9 aspect ratio
    const int physScreenHeight = qRound(diagMM / 1.975);
    const int physScreenWidth = qRound(physScreenHeight * 1.777);

    foreach (const QByteArray &line, lines)
        f.write(line + QByteArrayLiteral("\n"));

    if (includePhysSize)
        f.write(QByteArrayLiteral("env=QT_QPA_EGLFS_PHYSICAL_WIDTH=") + QByteArray::number(physScreenWidth)
                + QByteArrayLiteral("\nenv=QT_QPA_EGLFS_PHYSICAL_HEIGHT=") + QByteArray::number(physScreenHeight)
                + QByteArrayLiteral("\n"));
}

void PhysicalScreenSize::setSize(int inches)
{
    physScreenSizeInch = inches;
    physWriteTimer.start();
}

bool PhysicalScreenSize::enabled() const
{
    return physScreenSizeEnabled;
}

void PhysicalScreenSize::setEnabled(bool enable)
{
    physScreenSizeEnabled = enable;
    // Rewrite appcontroller.conf with or without the physical width/height lines.
    write(enable);
}

int B2QtDevice::physicalScreenSizeInch() const
{
    return physScreenSize()->size();
}

void B2QtDevice::setPhysicalScreenSizeInch(int inches)
{
    if (physScreenSize()->size() != inches) {
        physScreenSize()->setSize(inches);
        emit physicalScreenSizeInchChanged(inches);
    }
}

bool B2QtDevice::physicalScreenSizeOverride() const
{
    return physScreenSize()->enabled();
}

void B2QtDevice::setPhysicalScreenSizeOverride(bool enable)
{
    if (physScreenSize()->enabled() != enable) {
        physScreenSize()->setEnabled(enable);
        emit physicalScreenSizeOverrideChanged(enable);
    }
}

#include "b2qtdevice.moc"
