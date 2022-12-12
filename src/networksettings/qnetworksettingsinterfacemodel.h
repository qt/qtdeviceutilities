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
#ifndef QNETWORKSETTINGSINTERFACEMODEL_H
#define QNETWORKSETTINGSINTERFACEMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtNetworkSettings/qnetworksettingsglobal.h>

QT_BEGIN_NAMESPACE

class QNetworkSettingsInterface;
class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsInterfaceModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit QNetworkSettingsInterfaceModel(QObject *parent = nullptr);
    // from QAbstractItemModel
    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void append(QNetworkSettingsInterface* networkInterface);
    void insert(int row, QNetworkSettingsInterface* networkInterface);
    void remove(int row);
    QList<QNetworkSettingsInterface*> getModel();

    enum Roles {
        Type = Qt::UserRole + 1,
        Status,
        Name,
        Powered
    };

    void updated(int row);
    bool removeInterface(const QString &name);
private Q_SLOTS:
    void connectionStatusChanged();
    void poweredChanged();

private:
    void connectStateChanges(QNetworkSettingsInterface* item);

private:
    QList<QNetworkSettingsInterface*> m_items;
    QHash<int, QByteArray> m_roleNames;
};

QT_END_NAMESPACE

#endif // QNETWORKSETTINGSINTERFACEMODEL_H
