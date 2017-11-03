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
#ifndef BLUETOOTHDEVICE__P_H
#define BLUETOOTHDEVICE__P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QObject>
#include <QBluetoothLocalDevice>
#include <QtDBus>
#include "bluetoothdevice.h"

class OrgBluezDevice1Interface;
class OrgFreedesktopDBusObjectManagerInterface;
class NetConnmanTechnologyInterface;

class BluetoothDevicePrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(BluetoothDevice)
public:
    BluetoothDevice *q_ptr;
    BluetoothDevicePrivate(BluetoothDevice *parent);
    bool powered() const;
    void setPowered(const bool& aPowered);
    bool scanning() const;
    bool available() const;
    void setScanning(const bool& aScan);
    void requestPairing(const QString& address);
    void requestConnect(const QString& address);
    void requestDisconnect(const QString& address);
    DiscoveryModel* deviceModel() const;

public Q_SLOTS:
    void deviceStateChanged(QBluetoothLocalDevice::HostMode state);
    void scanFinished();
    //These are not yet signaled
    //See bug https://bugreports.qt.io/browse/QTBUG-38401
    void pairingDisplayConfirmation(const QBluetoothAddress & address, QString pin);
    void pairingDisplayPinCode(const QBluetoothAddress & address, QString pin);
    void pairingFinished(const QBluetoothAddress & address, QBluetoothLocalDevice::Pairing pairing);
    void deviceConnected(const QBluetoothAddress & address);
    void deviceDisconnected(const QBluetoothAddress & address);
    void getManagedObjectsFinished(QDBusPendingCallWatcher *watcher);

private:
    void updateConnectionStatuses();
    OrgBluezDevice1Interface* findPeerDevice(const QString& address);

private:
    QBluetoothLocalDevice* m_localDevice;
    bool m_powered;
    bool m_scanning;
    QString m_adapter;
    DiscoveryModel *m_deviceModel;
    OrgFreedesktopDBusObjectManagerInterface *m_manager;
    NetConnmanTechnologyInterface *m_technology;
};



#endif // BLUETOOTHDEVICE__P_H
