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
#include <discoverymodel.h>
#include "bluetoothdevice.h"
#include "bluetoothdevice_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class BluetoothDevice
    \inmodule QtDeviceUtilities

    \brief The BluetoothDevice class controls Bluetooth settings.

    The Bluetooth settings contain information about the local Bluetooth device,
    such as the device model and whether the device is powered on and available
    for connections with other Bluetooth devices.

    The Bluetooth device can scan for other Bluetooth devices in range and
    retrieve information about them. The Bluetooth device then uses the address
    of another device to attempt pairing to it, and connects to it if the
    pairing was successful.
*/

/*!
    \property BluetoothDevice::scanning
    \brief Whether the Bluetooth device is scanning for remote devices.
*/

/*!
    \property BluetoothDevice::powered
    \brief Whether the power in the Bluetooth device is on or off.
*/

/*!
    \property BluetoothDevice::available
    \brief Whether the Bluetooth device is available.
*/

/*!
    \property BluetoothDevice::deviceModel
    \brief The model of the Bluetooth device.
*/

/*!
    Creates a new Bluetooth device with the parent \a parent.
*/
BluetoothDevice::BluetoothDevice(QObject *parent) : QObject(parent)
  ,d_ptr(new BluetoothDevicePrivate(this))
{
}

/*!
    Returns \c true if the power is switched on in the Bluetooth device.
*/
bool BluetoothDevice::powered() const
{
    Q_D(const BluetoothDevice);
    return d->powered();
}

/*!
    Sets the powered state in the Bluetooth device to \a aPowered.
*/
void BluetoothDevice::setPowered(const bool& aPowered)
{
    Q_D(BluetoothDevice);
    d->setPowered(aPowered);
}

/*!
    Returns the model of the Bluetooth device.
*/
DiscoveryModel* BluetoothDevice::deviceModel() const
{
    Q_D(const BluetoothDevice);
    return d->deviceModel();
}

/*!
    Returns whether the Bluetooth device is scanning for remote devices.
*/
bool BluetoothDevice::scanning() const
{
    Q_D(const BluetoothDevice);
    return d->scanning();
}

/*!
    Sets scanning in the Bluetooth device to \a aScan.
*/
void BluetoothDevice::setScanning(const bool& aScan)
{
    Q_D(BluetoothDevice);
    d->setScanning(aScan);
}

/*!
    Starts the process of pairing to a remote device specified by \a address,
    and connects to it if the pairing was successful.

    \sa requestConnect
*/
void BluetoothDevice::requestPairing(const QString& address)
{
    Q_D(BluetoothDevice);
    d->requestPairing(address);
}

/*!
    Connects to the remote device specified by \a address.
*/
void BluetoothDevice::requestConnect(const QString& address)
{
    Q_D(BluetoothDevice);
    d->requestConnect(address);
}

/*!
    Disconnects from the remote device specified by \a address.
*/
void BluetoothDevice::requestDisconnect(const QString& address)
{
    Q_D(BluetoothDevice);
    d->requestDisconnect(address);
}

/*!
    Returns the availability of the Bluetooth device.
*/
bool BluetoothDevice::available() const
{
    Q_D(const BluetoothDevice);
    return d->available();
}

QT_END_NAMESPACE
