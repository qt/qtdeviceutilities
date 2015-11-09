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
#ifndef NETWORKSMODEL_H
#define NETWORKSMODEL_H

#include <QAbstractListModel>
#include "networkconfig.h"

QT_FORWARD_DECLARE_CLASS(NetworksModelPrivate)
QT_FORWARD_DECLARE_CLASS(QQmlPropertyMap)

class NetworkItem : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(NetworkState::States state READ state NOTIFY stateChanged)
    Q_PROPERTY(NetworkType::Types type READ type NOTIFY typeChanged)
    Q_PROPERTY(bool powered READ powered NOTIFY poweredChanged)

public:
    explicit NetworkItem(QObject* parent=0) :
        QObject(parent) {

    }

    QString name() const {
        return m_name;
    }

    void setName(const QString& aName)  {
        m_name = aName;
    }

    NetworkState::States state() const {
        return m_state.state();
    }

    NetworkType::Types type() const {
        return m_type.type();
    }

    void setState(const NetworkState::States aState) {
        m_state.setState(aState);
    }

    void setType(const NetworkType::Types aType) {
        m_type.setType(aType);
    }

    bool powered() const {
        return m_powered;
    }

    void setPowered(const bool aPowered) {
        m_powered = aPowered;
    }

signals:
    void nameChanged();
    void stateChanged();
    void typeChanged();
    void poweredChanged();

protected:
    QString m_name;
    NetworkState m_state;
    NetworkType m_type;
    bool m_powered;
    friend class NetworksModel;
};

class NetworksModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit NetworksModel(QObject *parent=0);
    virtual ~NetworksModel();
    // from QAbstractItemModel
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    void append(NetworkItem* networkItem);
    void insert(int row, NetworkItem* networkItem);
    QList<NetworkItem*> getModel();

    enum Roles {
        Type = Qt::UserRole + 1,
        Status,
        Name
    };

private:
    QList<NetworkItem*> m_items;
    QHash<int, QByteArray> m_roleNames;
};

#endif // NETWORKSMODEL_H
