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

BluetoothDevice::BluetoothDevice(QObject *parent) : QObject(parent)
  ,d_ptr(new BluetoothDevicePrivate(this))
{
}

bool BluetoothDevice::powered() const
{
    Q_D(const BluetoothDevice);
    return d->powered();
}

void BluetoothDevice::setPowered(const bool& aPowered)
{
    Q_D(BluetoothDevice);
    d->setPowered(aPowered);
}


DiscoveryModel* BluetoothDevice::deviceModel() const
{
    Q_D(const BluetoothDevice);
    return d->deviceModel();
}

bool BluetoothDevice::scanning() const
{
    Q_D(const BluetoothDevice);
    return d->scanning();
}

void BluetoothDevice::setScanning(const bool& aScan)
{
    Q_D(BluetoothDevice);
    d->setScanning(aScan);
}

void BluetoothDevice::requestPairing(const QString& address)
{
    Q_D(BluetoothDevice);
    d->requestPairing(address);
}

void BluetoothDevice::requestConnect(const QString& address)
{
    Q_D(BluetoothDevice);
    d->requestConnect(address);
}

void BluetoothDevice::requestDisconnect(const QString& address)
{
    Q_D(BluetoothDevice);
    d->requestDisconnect(address);
}

bool BluetoothDevice::available() const
{
    Q_D(const BluetoothDevice);
    return d->available();
}

QT_END_NAMESPACE
