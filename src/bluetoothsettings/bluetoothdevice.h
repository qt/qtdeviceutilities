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
#ifndef BLUETOOTHDEVICE_H
#define BLUETOOTHDEVICE_H

#include <QObject>
#include "discoverymodel.h"

QT_BEGIN_NAMESPACE

QT_FORWARD_DECLARE_CLASS(DiscoveryModel)
QT_FORWARD_DECLARE_CLASS(BluetoothDevicePrivate)

class Q_DECL_EXPORT BluetoothDevice : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool scanning READ scanning WRITE setScanning NOTIFY scanningChanged)
    Q_PROPERTY(bool powered READ powered WRITE setPowered NOTIFY poweredChanged)
    Q_PROPERTY(bool available READ available NOTIFY availabilityChanged)
    Q_PROPERTY(DiscoveryModel* deviceModel READ deviceModel CONSTANT)
public:
    explicit BluetoothDevice(QObject *parent = Q_NULLPTR);
    bool powered() const;
    void setPowered(const bool& aPowered);
    DiscoveryModel *deviceModel() const;
    bool scanning() const;
    void setScanning(const bool& aScan);
    bool available() const;
    Q_INVOKABLE void requestPairing(const QString& address);
    Q_INVOKABLE void requestConnect(const QString& address);
    Q_INVOKABLE void requestDisconnect(const QString& address);
Q_SIGNALS:
    void poweredChanged();
    void scanningChanged();
    void availabilityChanged();
protected:
    BluetoothDevicePrivate *d_ptr;
private:
Q_DISABLE_COPY(BluetoothDevice)
Q_DECLARE_PRIVATE(BluetoothDevice)
};

QT_END_NAMESPACE

#endif // BLUETOOTHDEVICE_H
