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

/*!
    \class DisplaySettings
    \inmodule QtDeviceUtilities

    \brief The DisplaySettings class specifies display settings.

    The Boot to Qt demo launcher and Qt Quick Controls scale automatically to
    screens of different sizes, from 7-inch touch screens to 60-inch or larger
    television screens to ensure readability and touch friendliness. For this,
    Qt has to know the physical dimensions of the screen. By default, it tries
    to query these values from the framebuffer devices. However, many kernel
    drivers do not provide this information.

    The display settings enable overriding and manually setting the physical
    screen size.

    In addition, you can set the display brightness. That is, the intensity of
    the backlight.
*/

/*!
    \property DisplaySettings::displayBrightness
    \brief The display brightness.

    A value of \c 255 requests the maximum brightness, while that of \c 0
    requests the minimum (typically, the backlight turned off).
*/


/*!
    \property DisplaySettings::physicalScreenSizeInch
    \brief The physical screen size in inches.
*/

/*!
    \property DisplaySettings::physicalScreenWidthMm
    \brief The physical screen width in millimeters.
*/

/*!
    \property DisplaySettings::physicalScreenHeightMm
    \brief The physical screen height in millimeters.
*/

/*!
    \property DisplaySettings::physicalScreenSizeOverride
    \brief Whether to use the value set for the physical screen size.
*/

/*!
    Creates a new display settings object with the parent \a parent.
*/
DisplaySettings::DisplaySettings(QObject *parent)
    : QObject(parent)
    ,d_ptr(new DisplaySettingsPrivate(this))
{
}

/*!
    Deletes the display settings object.
*/
DisplaySettings::~DisplaySettings()
{
}


/*!
    Sets the display brightness to \a v.

    Returns \c true on success.
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

/*!
    Returns the physical screen size in inches.
*/
int DisplaySettings::physicalScreenSizeInch() const
{
    Q_D(const DisplaySettings);
    return d->physicalScreenSizeInch();
}

/*!
    Returns the physical screen width in millimeters.
*/
int DisplaySettings::physicalScreenWidthMm() const
{
    Q_D(const DisplaySettings);
    return d->physicalScreenWidthMm();
}

/*!
    Returns the physical screen height in millimeters.
*/
int DisplaySettings::physicalScreenHeightMm() const
{
    Q_D(const DisplaySettings);
    return d->physicalScreenHeightMm();
}

/*!
    Sets the physical screen size to \a inches.
*/
void DisplaySettings::setPhysicalScreenSizeInch(int inches)
{
    Q_D(DisplaySettings);
    d->setPhysicalScreenSizeInch(inches);
}

/*!
    Sets the physical screen width to \a newWidth.
*/
void DisplaySettings::setPhysicalScreenWidthMm(int newWidth)
{
    Q_D(DisplaySettings);
    d->setPhysicalScreenWidthMm(newWidth);
}

/*!
    Sets the physical screen height to \a newHeight.
*/
void DisplaySettings::setPhysicalScreenHeightMm(int newHeight)
{
    Q_D(DisplaySettings);
    d->setPhysicalScreenHeightMm(newHeight);
}

/*!
    Returns whether the value set for the physical screen size is used.

    \sa physicalScreenSizeInch()
*/
bool DisplaySettings::physicalScreenSizeOverride() const
{
    Q_D(const DisplaySettings);
    return d->physicalScreenSizeOverride();
}

/*!
    Sets whether the value set for the physical screen size is used to
    \a enable.

    \sa physicalScreenSizeInch()
*/
void DisplaySettings::setPhysicalScreenSizeOverride(bool enable)
{
    Q_D(DisplaySettings);
    d->setPhysicalScreenSizeOverride(enable);
}

QT_END_NAMESPACE
