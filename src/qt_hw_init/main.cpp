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
