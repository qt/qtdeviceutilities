#include "../utils/qdroidutils.h"
#include <QDebug>

static void setMaxVolume(QDroidUtils *utils)
{
    // Set the audio orientation to something to force the HW driver to reconfigure
    // audio routing (workaround for bug on Nexus 7)
    utils->setOrientationForAudioSystem(QDroidUtils::LandscapeAudioOrientation);
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
