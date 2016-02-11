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
#ifndef DISCOVERYMODEL_H
#define DISCOVERYMODEL_H


#include <QObject>
#include <QAbstractListModel>
#include <QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>

class Q_DECL_EXPORT BtDeviceItem : public QObject
{
    Q_OBJECT
    Q_ENUMS(DeviceType)
    Q_PROPERTY(QString address READ address CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(DeviceType type READ type CONSTANT)
public:
    explicit BtDeviceItem(const QBluetoothDeviceInfo& id, QObject *parent=0);
    //The list of device type we want to show the icon
    enum DeviceType {
        Phone,
        Computer,
        Mouse,
        Keyboard,
        Headphones,
        Microphone,
        Camera,
        Camcorder,
        Clock,
        HealthDevice,
        GenericDevice=1000
    };
    QString name() const;
    QString address() const;
    DeviceType type() const;
    bool connected() const;
    void setConnected(bool aConnected);

signals:
    void connectedChanged();

protected:
    DeviceType getDeviceType(const QBluetoothDeviceInfo::MajorDeviceClass major,
                                  const quint8 minor) const;
    DeviceType getComputerDeviceType(const quint8 minor) const;
    DeviceType getAudioDeviceType(const quint8 minor) const;
    DeviceType getPeripheralDeviceType(const quint8 minor) const;
    DeviceType getImagingDeviceType(const quint8 minor) const;
    DeviceType getHealthDeviceType(const quint8 minor) const;
    DeviceType getPhoneDeviceType(const quint8 minor) const;

private:
    QString m_name;
    QString m_address;
    bool m_connected;
    DeviceType m_type;
};

class Q_DECL_EXPORT DiscoveryModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(DeviceType)
public:
    explicit DiscoveryModel(QObject *parent=0);
    virtual ~DiscoveryModel();
    // from QAbstractItemModel
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    void setConnected(const QString& aAddress, bool connected);
    void scanDevices();

    enum Roles {
        Name = Qt::UserRole,
        Address,
        Type,
        Connected
    };

signals:
    void scanFinished();

private slots:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
private:
    QList<BtDeviceItem*> m_items;
    QHash<int, QByteArray> m_roleNames;
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
};
#endif // DISCOVERYMODEL_H
