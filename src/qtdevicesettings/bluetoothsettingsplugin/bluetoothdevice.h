/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef BLUETOOTHDEVICE_H
#define BLUETOOTHDEVICE_H

#include <QObject>
#include <QBluetoothLocalDevice>

class DiscoveryModel;

class BluetoothDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool scanning READ scanning WRITE setScanning NOTIFY scanningChanged)
    Q_PROPERTY(bool powered READ powered WRITE setPowered NOTIFY poweredChanged)
    Q_PROPERTY(QObject* deviceModel READ deviceModel CONSTANT)
public:
    explicit BluetoothDevice(QObject *parent = 0);
    bool powered() const;
    void setPowered(const bool& aPowered);
    QObject* deviceModel() const;
    bool scanning() const;
    void setScanning(const bool& aScan);
    Q_INVOKABLE void requestPairing(const QString& address);
    Q_INVOKABLE void requestConnect(const QString& address);
    Q_INVOKABLE void requestDisconnect(const QString& address);
signals:
    void poweredChanged();
    void scanningChanged();

public slots:
    void deviceStateChanged(QBluetoothLocalDevice::HostMode state);
    void scanFinished();
    //These are not yet signaled
    //See bug https://bugreports.qt.io/browse/QTBUG-38401
    void pairingDisplayConfirmation(const QBluetoothAddress & address, QString pin);
    void pairingDisplayPinCode(const QBluetoothAddress & address, QString pin);
    void pairingFinished(const QBluetoothAddress & address, QBluetoothLocalDevice::Pairing pairing);
    void deviceConnected(const QBluetoothAddress & address);
    void deviceDisconnected(const QBluetoothAddress & address);

private:
    void updateConnectionStatuses();

private:
    QBluetoothLocalDevice* m_localDevice;
    DiscoveryModel *m_deviceModel;
    bool m_powered;
    bool m_scanning;
};

#endif // BLUETOOTHDEVICE_H
