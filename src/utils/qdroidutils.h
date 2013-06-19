#ifndef QDROIDUTILS_H
#define QDROIDUTILS_H

#include <qobject.h>

class Q_DECL_EXPORT QDroidUtils : public QObject
{
    Q_OBJECT
    Q_ENUMS(AudioStreamType)
public:
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

    QDroidUtils(QObject* parent = 0) : QObject(parent)
    {
    }
    ~QDroidUtils()
    {
    }

    //### TBD: make an instance() method, for singleton use from C++ ?
    //e.g. connect(myobj, mysig, QDroidUtils::instance(), slot(rebootSystem());

public Q_SLOTS:
    void rebootSystem();
    void powerOffSystem();

    void setMasterVolume(int volume);
    void setMasterMute(bool mute);
    void setStreamVolume(AudioStreamType stream, int volume);
    void setStreamMute(AudioStreamType stream, bool mute);

    bool setDisplayBrightness(quint8 value);

    QString getIPAddress();
    QString getHostname();
    bool setHostname(QString hostname);
};

#endif // QDROIDUTILS_H
