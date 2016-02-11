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

#include <QBluetoothAddress>
#include "discoverymodel.h"

BtDeviceItem::BtDeviceItem(const QBluetoothDeviceInfo& bt, QObject *parent)
    : QObject(parent)
    ,m_name(bt.name())
    ,m_address(bt.address().toString())
    ,m_connected(false)
{
    m_type = getDeviceType(bt.majorDeviceClass(), bt.minorDeviceClass());
}

QString BtDeviceItem::name() const
{
    return m_name;
}

QString BtDeviceItem::address() const
{
    return m_address;
}

bool BtDeviceItem::connected() const
{
    return m_connected;
}

void BtDeviceItem::setConnected(bool aConnected)
{
    m_connected = aConnected;
    emit connectedChanged();
}

BtDeviceItem::DeviceType BtDeviceItem::type() const
{
    return m_type;
}

BtDeviceItem::DeviceType BtDeviceItem::getDeviceType(const QBluetoothDeviceInfo::MajorDeviceClass major, const quint8 minor) const
{
    switch (major) {
        case QBluetoothDeviceInfo::ComputerDevice:
            return getComputerDeviceType(minor);
        break;
        case QBluetoothDeviceInfo::PhoneDevice:
            return getPhoneDeviceType(minor);
        break;
        case QBluetoothDeviceInfo::AudioVideoDevice:
            return getAudioDeviceType(minor);
        break;
        case QBluetoothDeviceInfo::PeripheralDevice:
            return getPeripheralDeviceType(minor);
        break;
        case QBluetoothDeviceInfo::ImagingDevice:
            return getImagingDeviceType(minor);
        break;
        default:
            return GenericDevice;
    }
    return GenericDevice;
}

BtDeviceItem::DeviceType BtDeviceItem::getComputerDeviceType(const quint8 minor) const
{
    Q_UNUSED(minor);
    return Computer;
}

BtDeviceItem::DeviceType BtDeviceItem::getAudioDeviceType(const quint8 minor) const
{
    switch (minor) {
    case QBluetoothDeviceInfo::Microphone:
        return Microphone;
        break;
    case QBluetoothDeviceInfo::WearableHeadsetDevice:
    case QBluetoothDeviceInfo::Headphones:
        return Headphones;
        break;
    case QBluetoothDeviceInfo::Camcorder:
    case QBluetoothDeviceInfo::VideoCamera:
        return Camcorder;
        break;
    default:
        return GenericDevice;
        break;
    }
}

BtDeviceItem::DeviceType BtDeviceItem::getPeripheralDeviceType(const quint8 minor) const
{
    switch (minor) {
    case QBluetoothDeviceInfo::KeyboardPeripheral:
        return Keyboard;
        break;
    case QBluetoothDeviceInfo::PointingDevicePeripheral:
        return Mouse;
        break;
    default:
        return GenericDevice;
        break;
    }
}

BtDeviceItem::DeviceType BtDeviceItem::getImagingDeviceType(const quint8 minor) const
{
    switch (minor) {
    case QBluetoothDeviceInfo::ImageCamera:
        return Camera;
        break;
    default:
        return GenericDevice;
        break;
    }
}

BtDeviceItem::DeviceType BtDeviceItem::getPhoneDeviceType(const quint8 minor) const
{
    Q_UNUSED(minor);
    return Phone;
}


DiscoveryModel::DiscoveryModel(QObject *parent)
    : QAbstractListModel(parent)
    ,m_discoveryAgent(new QBluetoothDeviceDiscoveryAgent(this))
{
    m_roleNames.insert(Qt::UserRole, "modelData");
    m_roleNames.insert(Address, "address");
    m_roleNames.insert(Name, "name");
    m_roleNames.insert(Type, "type");
    m_roleNames.insert(Connected, "connected");

    connect(m_discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
            this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));

    connect(m_discoveryAgent, SIGNAL(finished()),
            this, SIGNAL(scanFinished()));

}

void DiscoveryModel::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
   beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
   BtDeviceItem *item = new BtDeviceItem(device);
   m_items.append(item);
   endInsertRows();
}

DiscoveryModel::~DiscoveryModel()
{

}

void DiscoveryModel::scanDevices()
{
    m_discoveryAgent->start();
}

QHash<int, QByteArray> DiscoveryModel::roleNames() const
{
    return m_roleNames;
}

int DiscoveryModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return m_items.count();
}

QVariant DiscoveryModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid()) return QVariant();

    BtDeviceItem *item = m_items[index.row()];

    switch (role) {
    case DiscoveryModel::Name:
        return item->name();
        break;
    case DiscoveryModel::Address:
        return item->address();
        break;
    case DiscoveryModel::Type:
        return item->type();
        break;
    case DiscoveryModel::Connected:
        return item->connected();
    default:
        return "";
    }
}

void DiscoveryModel::setConnected(const QString &aAddress, bool connected)
{
    bool found = false;
    int i = 0;
    QVector<int> role;
    role.append(DiscoveryModel::Connected);
    foreach (BtDeviceItem *item, m_items) {
        if (item->address() == aAddress) {
            item->setConnected(connected);
            found = true;
            break;
        }
        i++;
    }

    if (found)
        emit dataChanged(index(i, 0), index(i, 0), role);
}
