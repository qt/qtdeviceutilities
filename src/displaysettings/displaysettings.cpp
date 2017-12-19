/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "displaysettings.h"
#include "displaysettings_p.h"

QT_BEGIN_NAMESPACE

DisplaySettings::DisplaySettings(QObject *parent)
    : QObject(parent)
    ,d_ptr(new DisplaySettingsPrivate(this))
{
}

DisplaySettings::~DisplaySettings()
{
}


/*!
 * Sets the display brightness (i.e. the intensity of the backlight)
 * to \a value. A value of 255 requests maximum brightness, while 0 requests
 * minimum (typically, the backlight turned off).
 *
 * Returns true on success.
 */
bool DisplaySettings::setDisplayBrightness(int v)
{
    Q_D(DisplaySettings);
    return d->setDisplayBrightness(v);
}


/*!
 * Returns the current backlight intensity.
 * \sa setDisplayBrightness
 */
int DisplaySettings::displayBrightness()
{
    Q_D(DisplaySettings);
    return d->displayBrightness();
}


int DisplaySettings::physicalScreenSizeInch() const
{
    Q_D(const DisplaySettings);
    return d->physicalScreenSizeInch();
}

int DisplaySettings::physicalScreenWidthMm() const
{
    Q_D(const DisplaySettings);
    return d->physicalScreenWidthMm();
}

int DisplaySettings::physicalScreenHeightMm() const
{
    Q_D(const DisplaySettings);
    return d->physicalScreenHeightMm();
}

void DisplaySettings::setPhysicalScreenSizeInch(int inches)
{
    Q_D(DisplaySettings);
    d->setPhysicalScreenSizeInch(inches);
}

void DisplaySettings::setPhysicalScreenWidthMm(int newWidth)
{
    Q_D(DisplaySettings);
    d->setPhysicalScreenWidthMm(newWidth);
}

void DisplaySettings::setPhysicalScreenHeightMm(int newHeight)
{
    Q_D(DisplaySettings);
    d->setPhysicalScreenHeightMm(newHeight);
}

bool DisplaySettings::physicalScreenSizeOverride() const
{
    Q_D(const DisplaySettings);
    return d->physicalScreenSizeOverride();
}

void DisplaySettings::setPhysicalScreenSizeOverride(bool enable)
{
    Q_D(DisplaySettings);
    d->setPhysicalScreenSizeOverride(enable);
}

QT_END_NAMESPACE
