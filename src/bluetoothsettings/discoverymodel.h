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
#ifndef DISCOVERYMODEL_H
#define DISCOVERYMODEL_H


#include <QObject>
#include <QAbstractListModel>
#include <QBluetoothDeviceInfo>
#include <QBluetoothDeviceDiscoveryAgent>

QT_BEGIN_NAMESPACE

class Q_DECL_EXPORT BtDeviceItem : public QObject
{
    Q_OBJECT
    Q_ENUMS(DeviceType)
    Q_PROPERTY(QString address READ address CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(DeviceType type READ type CONSTANT)
public:
    explicit BtDeviceItem(const QBluetoothDeviceInfo& id, QObject *parent = Q_NULLPTR);
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

Q_SIGNALS:
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
    explicit DiscoveryModel(QObject *parent = Q_NULLPTR);
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

Q_SIGNALS:
    void scanFinished();

private Q_SLOTS:
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
private:
    QList<BtDeviceItem*> m_items;
    QHash<int, QByteArray> m_roleNames;
    QBluetoothDeviceDiscoveryAgent *m_discoveryAgent;
};

QT_END_NAMESPACE

#endif // DISCOVERYMODEL_H
