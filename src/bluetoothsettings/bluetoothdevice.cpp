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
#include "bluez/bluetoothdevice_p.h"

BluetoothDevice::BluetoothDevice(QObject *parent) : QObject(parent)
   ,m_localDevice(new QBluetoothLocalDevice(this))
   ,m_deviceModel(new DiscoveryModel(this))
   ,m_powered(false)
   ,m_scanning(true)
{
    m_powered = m_localDevice->hostMode() != QBluetoothLocalDevice::HostPoweredOff;

    connect(m_localDevice, &QBluetoothLocalDevice::hostModeStateChanged, this, &BluetoothDevice::deviceStateChanged);
    connect(m_localDevice, &QBluetoothLocalDevice::deviceConnected, this, &BluetoothDevice::deviceConnected);
    connect(m_localDevice, &QBluetoothLocalDevice::deviceDisconnected, this, &BluetoothDevice::deviceDisconnected);
    connect(m_deviceModel, &DiscoveryModel::scanFinished, this, &BluetoothDevice::scanFinished);

    if (m_powered) {
        m_deviceModel->scanDevices();
    }

}

void BluetoothDevice::deviceStateChanged(QBluetoothLocalDevice::HostMode state)
{
    m_powered = state != QBluetoothLocalDevice::HostPoweredOff;
    emit poweredChanged();
}

bool BluetoothDevice::powered() const
{
    return m_powered;
}

void BluetoothDevice::setPowered(const bool& aPowered)
{
    if (aPowered) {
        m_localDevice->powerOn();
    }
    else {
        m_localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    }
}

QObject* BluetoothDevice::deviceModel() const
{
    return static_cast<QObject*>(m_deviceModel);
}

void BluetoothDevice::scanFinished()
{
    m_scanning = false;
    emit scanningChanged();
    updateConnectionStatuses();
}

bool BluetoothDevice::scanning() const
{
    return m_scanning;
}

void BluetoothDevice::setScanning(const bool& aScan)
{
    if (m_scanning && !aScan) {
        //TODO m_deviceModel->cancel();
    }
    else if (aScan && !m_scanning) {
        m_deviceModel->scanDevices();
        m_scanning = true;
        emit scanningChanged();
    }
}

void BluetoothDevice::updateConnectionStatuses()
{
    QList<QBluetoothAddress> connectedDevices =
            m_localDevice->connectedDevices();

    foreach (QBluetoothAddress addr, connectedDevices) {
        m_deviceModel->setConnected(addr.toString(), true);
    }
}

void BluetoothDevice::requestPairing(const QString& address)
{
    QBluetoothAddress addr(address);
    m_localDevice->requestPairing(addr, QBluetoothLocalDevice::Paired);
    connect(m_localDevice, &QBluetoothLocalDevice::pairingDisplayConfirmation, this, &BluetoothDevice::pairingDisplayConfirmation);

    connect(m_localDevice, &QBluetoothLocalDevice::pairingDisplayPinCode, this, &BluetoothDevice::pairingDisplayPinCode);

    connect(m_localDevice, &QBluetoothLocalDevice::pairingFinished, this, &BluetoothDevice::pairingFinished);
}

void BluetoothDevice::requestConnect(const QString &address)
{
    QScopedPointer<BluetoothDevicePrivate> connectionHandler(new BluetoothDevicePrivate(address));
    connectionHandler->connectDevice();
}

void BluetoothDevice::requestDisconnect(const QString& address)
{
    QScopedPointer<BluetoothDevicePrivate> connectionHandler(new BluetoothDevicePrivate(address));
    connectionHandler->disconnectDevice();
}

void BluetoothDevice::pairingDisplayConfirmation(const QBluetoothAddress & address, QString pin)
{
    Q_UNUSED(address);
    Q_UNUSED(pin);
}

void BluetoothDevice::pairingDisplayPinCode(const QBluetoothAddress & address, QString pin)
{
    Q_UNUSED(address);
    Q_UNUSED(pin);
}

void BluetoothDevice::pairingFinished(const QBluetoothAddress & address, QBluetoothLocalDevice::Pairing pairing)
{
    if (pairing == QBluetoothLocalDevice::Paired) {
        requestConnect(address.toString());
    }
}

void BluetoothDevice::deviceConnected(const QBluetoothAddress & address)
{
    m_deviceModel->setConnected(address.toString(), true);
}

void BluetoothDevice::deviceDisconnected(const QBluetoothAddress & address)
{
    m_deviceModel->setConnected(address.toString(), false);
}
