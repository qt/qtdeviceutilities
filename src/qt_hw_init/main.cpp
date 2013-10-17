#include "../utils/qdroidutils.h"
#include <QDebug>

static void setMaxVolume(QDroidUtils *utils)
{
    utils->setMasterVolume(100);
    utils->setMasterMute(false);
    utils->setStreamVolume(QDroidUtils::SystemAudioStream, 100);
    utils->setStreamVolume(QDroidUtils::MusicAudioStream, 100);
    utils->setStreamVolume(QDroidUtils::NotificationAudioStream, 100);
    utils->setStreamVolume(QDroidUtils::EnforcedAudibleAudioStream, 100);
}

static void setDisplayBrightness(QDroidUtils *utils)
{
    utils->setDisplayBrightness(255);
}

static void showValues(QDroidUtils *utils)
{
    qDebug() << "Master volume:" << utils->masterVolume();
    qDebug() << "Master mute:" << utils->masterMute();
    qDebug() << "System audio volume:" << utils->streamVolume(QDroidUtils::SystemAudioStream);
    qDebug() << "System audio mute:" << utils->streamMute(QDroidUtils::SystemAudioStream);
    qDebug() << "Music audio volume:" << utils->streamVolume(QDroidUtils::MusicAudioStream);
    qDebug() << "Music audio mute:" << utils->streamMute(QDroidUtils::MusicAudioStream);
    qDebug() << "Notification audio volume:" << utils->streamVolume(QDroidUtils::NotificationAudioStream);
    qDebug() << "Notification audio mute:" << utils->streamMute(QDroidUtils::NotificationAudioStream);
    qDebug() << "EnforcedAudible audio volume:" << utils->streamVolume(QDroidUtils::EnforcedAudibleAudioStream);
    qDebug() << "EnforcedAudible audio mute:" << utils->streamMute(QDroidUtils::EnforcedAudibleAudioStream);
}

int main(int, char *[])
{
    QDroidUtils utils;

    qDebug() << "Audio settings before change:";
    showValues(&utils);

    setMaxVolume(&utils);
    setDisplayBrightness(&utils);

    qDebug() << "Audio settings after change:";
    showValues(&utils);

    return 0;
}
