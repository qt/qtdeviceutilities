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
#include "qlocaldevice.h"
#include <unistd.h>
#include <sys/reboot.h>

QT_BEGIN_NAMESPACE

/*!
    \module QtLocalDeviceSettings
    \qtvariable localdevice
    \ingroup qtdevice-utilities-cpp-modules
    \ingroup modules
    \title QtLocalDeviceSettings C++ Classes
    \brief Provides functionality for controlling a local device settings.

    To use classes from this module, add this directive into the C++ files:

    \code
    #include <QtLocalDevice>
    \endcode

    To link against the corresponding C++ libraries, add the following to your
    qmake project file:

    \code
    QT += localdevice
    \endcode
*/

/*!
    \class QLocalDevice
    \inmodule QtLocalDeviceSettings

    \brief The QLocalDevice class provides utility functions for controlling an
    embedded device.

    The methods in this class enable shutting down and rebooting an embedded
    device.
*/

/*!
    Creates a new local device with the parent \a parent.
*/
QLocalDevice::QLocalDevice(QObject *parent)
    : QObject(parent)
{
}

/*!
    Deletes the local device.
*/
QLocalDevice::~QLocalDevice()
{
}

/*!
 * Reboots the system. Does not return.
 *
 * \sa powerOff()
 */
void QLocalDevice::reboot()
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
void QLocalDevice::powerOff()
{
    sync();
    ::reboot(RB_POWER_OFF);
    qWarning("powerOff returned");
}

QT_END_NAMESPACE
