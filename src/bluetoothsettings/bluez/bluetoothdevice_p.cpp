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
#include "discoverymodel.h"
#include "bluetoothdevice_p.h"
#include "datatypes.h"
#include "objectmanager_interface.cpp"
#include "moc_objectmanager_interface.cpp"
#include "device1_interface.h"
#include "connman_technology_interface.h"
#include "connmancommon.h"

QT_BEGIN_NAMESPACE

BluetoothDevicePrivate::BluetoothDevicePrivate(BluetoothDevice *parent) : QObject(parent)
    ,q_ptr(parent)
    ,m_localDevice(Q_NULLPTR)
    ,m_powered(false)
    ,m_scanning(false)
    ,m_adapter(QStringLiteral(""))
    ,m_deviceModel(new DiscoveryModel(this))
{
    qDBusRegisterMetaType<InterfaceList>();
    qDBusRegisterMetaType<ManagedObjectList>();

    m_manager = new OrgFreedesktopDBusObjectManagerInterface(QStringLiteral("org.bluez"),
                                                     QStringLiteral("/"),
                                                     QDBusConnection::systemBus(), this);
    m_technology = new NetConnmanTechnologyInterface(QStringLiteral("net.connman"),
                                                     QStringLiteral("/net/connman/technology/bluetooth"),
                                                     QDBusConnection::systemBus(), this);
    QDBusPendingReply<ManagedObjectList> reply = m_manager->GetManagedObjects();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished,
            this, &BluetoothDevicePrivate::getManagedObjectsFinished, Qt::QueuedConnection);
}

void BluetoothDevicePrivate::deviceStateChanged(QBluetoothLocalDevice::HostMode state)
{
    Q_Q(BluetoothDevice);
    m_powered = state != QBluetoothLocalDevice::HostPoweredOff;
    emit q->poweredChanged();
}

bool BluetoothDevicePrivate::powered() const
{
    return m_powered;
}

void BluetoothDevicePrivate::setPowered(const bool& aPowered)
{
    if (!m_localDevice)
        return;

    m_technology->SetProperty(PropertyPowered, QDBusVariant(QVariant(aPowered)));

    if (aPowered) {
        m_localDevice->powerOn();
    }
    else {
        m_localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);
    }
}

DiscoveryModel* BluetoothDevicePrivate::deviceModel() const
{
    return m_deviceModel;
}

void BluetoothDevicePrivate::scanFinished()
{
    Q_Q(BluetoothDevice);
    m_scanning = false;
    emit q->scanningChanged();
    updateConnectionStatuses();
}

bool BluetoothDevicePrivate::scanning() const
{
    return m_scanning;
}

void BluetoothDevicePrivate::setScanning(const bool& aScan)
{
    Q_Q(BluetoothDevice);
    if (m_scanning && !aScan) {
        m_deviceModel->stopScanning();
    }
    else if (aScan && !m_scanning) {
        m_deviceModel->scanDevices();
    }
    m_scanning = aScan;
    emit q->scanningChanged();
}

bool BluetoothDevicePrivate::available() const
{
    return (!m_adapter.isEmpty());
}

void BluetoothDevicePrivate::updateConnectionStatuses()
{
    QList<QBluetoothAddress> connectedDevices =
            m_localDevice->connectedDevices();

    foreach (QBluetoothAddress addr, connectedDevices) {
        m_deviceModel->setConnected(addr.toString(), true);
    }
}

void BluetoothDevicePrivate::requestPairing(const QString& address)
{
    QBluetoothAddress addr(address);
    if (m_localDevice) {
        m_localDevice->requestPairing(addr, QBluetoothLocalDevice::Paired);
        connect(m_localDevice, &QBluetoothLocalDevice::pairingDisplayConfirmation, this, &BluetoothDevicePrivate::pairingDisplayConfirmation);

        connect(m_localDevice, &QBluetoothLocalDevice::pairingDisplayPinCode, this, &BluetoothDevicePrivate::pairingDisplayPinCode);

        connect(m_localDevice, &QBluetoothLocalDevice::pairingFinished, this, &BluetoothDevicePrivate::pairingFinished);
    }
}

void BluetoothDevicePrivate::requestConnect(const QString &address)
{
    OrgBluezDevice1Interface *dev = findPeerDevice(address);
    if (dev) {
        dev->Connect();
        dev->deleteLater();
    }
}

void BluetoothDevicePrivate::requestDisconnect(const QString& address)
{
    OrgBluezDevice1Interface *dev = findPeerDevice(address);
    if (dev) {
        dev->Disconnect();
        dev->deleteLater();
    }
}

void BluetoothDevicePrivate::pairingDisplayConfirmation(const QBluetoothAddress & address, QString pin)
{
    Q_UNUSED(address);
    Q_UNUSED(pin);
}

void BluetoothDevicePrivate::pairingDisplayPinCode(const QBluetoothAddress & address, QString pin)
{
    Q_UNUSED(address);
    Q_UNUSED(pin);
}

void BluetoothDevicePrivate::pairingFinished(const QBluetoothAddress & address, QBluetoothLocalDevice::Pairing pairing)
{
    if (pairing == QBluetoothLocalDevice::Paired) {
        requestConnect(address.toString());
    }
}

void BluetoothDevicePrivate::deviceConnected(const QBluetoothAddress & address)
{
    m_deviceModel->setConnected(address.toString(), true);
}

void BluetoothDevicePrivate::deviceDisconnected(const QBluetoothAddress & address)
{
    m_deviceModel->setConnected(address.toString(), false);
}

void BluetoothDevicePrivate::getManagedObjectsFinished(QDBusPendingCallWatcher *watcher)
{
    Q_Q(BluetoothDevice);

    QDBusPendingReply<ManagedObjectList> reply = *watcher;
    watcher->deleteLater();
    if (reply.isError()) {
        return;
    }

    //Find adapter
    ManagedObjectList managedObjectList = reply.value();
    for (ManagedObjectList::const_iterator it = managedObjectList.constBegin(); it != managedObjectList.constEnd(); ++it) {
        const InterfaceList &ifaceList = it.value();
            for (InterfaceList::const_iterator jt = ifaceList.constBegin(); jt != ifaceList.constEnd(); ++jt) {
            const QString &iface = jt.key();
            const QVariantMap &ifaceValues = jt.value();

            if (iface == QStringLiteral("org.bluez.Adapter1")) {
                m_adapter = ifaceValues[QStringLiteral("Address")].toString();
                break;
            }
       }
    }

    if (!m_adapter.isEmpty()) {
        m_localDevice = new QBluetoothLocalDevice(this);
        m_powered = m_localDevice->hostMode() != QBluetoothLocalDevice::HostPoweredOff;

        connect(m_localDevice, &QBluetoothLocalDevice::hostModeStateChanged, this, &BluetoothDevicePrivate::deviceStateChanged);
        connect(m_localDevice, &QBluetoothLocalDevice::deviceConnected, this, &BluetoothDevicePrivate::deviceConnected);
        connect(m_localDevice, &QBluetoothLocalDevice::deviceDisconnected, this, &BluetoothDevicePrivate::deviceDisconnected);
        connect(m_deviceModel, &DiscoveryModel::scanFinished, this, &BluetoothDevicePrivate::scanFinished);

        if (m_powered) {
            emit q->poweredChanged();
            if (!m_scanning) {
                m_deviceModel->scanDevices();
                m_scanning = true;
                emit q->scanningChanged();
            }
        }

        emit q->availabilityChanged();
    }
}

OrgBluezDevice1Interface* BluetoothDevicePrivate::findPeerDevice(const QString &address)
{
    QDBusPendingReply<ManagedObjectList> reply = m_manager->GetManagedObjects();
    reply.waitForFinished();
    if (reply.isError()) {
        qWarning() << "Failed to get objects";
        return NULL;
    }

    ManagedObjectList managedObjectList = reply.value();
    for (ManagedObjectList::const_iterator it = managedObjectList.constBegin(); it != managedObjectList.constEnd(); ++it) {
        const QDBusObjectPath &path = it.key();

        const InterfaceList &ifaceList = it.value();
            for (InterfaceList::const_iterator jt = ifaceList.constBegin(); jt != ifaceList.constEnd(); ++jt) {
            const QString &iface = jt.key();
            const QVariantMap &ifaceValues = jt.value();
            if (iface == QStringLiteral("org.bluez.Device1")) {
                if (ifaceValues[QStringLiteral("Address")] == address) {
                    OrgBluezDevice1Interface *devIf = new OrgBluezDevice1Interface(QStringLiteral("org.bluez"), path.path(), QDBusConnection::systemBus());
                    return devIf;
                }
            }
        }
    }
    return NULL;
}

QT_END_NAMESPACE
