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
#include "bluetoothdevice_p.h"
#include "datatypes.h"
#include "objectmanager_interface.cpp"
#include "moc_objectmanager_interface.cpp"
#include "device1_interface.h"


BluetoothDevicePrivate::BluetoothDevicePrivate(const QString& address, QObject *parent)
    :QObject(parent)
    ,m_address(address)
{

}

OrgBluezDevice1Interface* BluetoothDevicePrivate::findDevice()
{
    OrgFreedesktopDBusObjectManagerInterface manager(QStringLiteral("org.bluez"),
                                                     QStringLiteral("/"),
                                                     QDBusConnection::systemBus());
    QDBusPendingReply<ManagedObjectList> reply = manager.GetManagedObjects();
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
                if (ifaceValues[QStringLiteral("Address")] == m_address) {
                    OrgBluezDevice1Interface *devIf = new OrgBluezDevice1Interface(QStringLiteral("org.bluez"), path.path(), QDBusConnection::systemBus());
                    return devIf;
                }
            }
        }
    }
    return NULL;
}

void BluetoothDevicePrivate::connectDevice()
{
    OrgBluezDevice1Interface *dev = findDevice();
    if (dev) {
        dev->Connect();
        dev->deleteLater();
    }
}

void BluetoothDevicePrivate::disconnectDevice()
{
    OrgBluezDevice1Interface *dev = findDevice();
    if (dev) {
        dev->Disconnect();
        dev->deleteLater();
    }
}
