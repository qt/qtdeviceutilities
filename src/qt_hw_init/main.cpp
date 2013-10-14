#include "../utils/qdroidutils.h"

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

int main(int, char *[])
{
    QDroidUtils utils;

    setMaxVolume(&utils);
    setDisplayBrightness(&utils);

    return 0;
}
