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
#ifndef QNETWORKSETTINGSADDRESSMODEL_H
#define QNETWORKSETTINGSADDRESSMODEL_H

#include <QStringListModel>

class Q_DECL_EXPORT QNetworkSettingsAddressModel : public QStringListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit QNetworkSettingsAddressModel(QObject *parent = 0);
    explicit QNetworkSettingsAddressModel(const QStringList &strings, QObject *parent = 0);
    Q_INVOKABLE void append(const QString& address);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void resetChanges();
    void setStringList(const QStringList &addresses);
    int count() const;

signals:
    void countChanged();
private:
    QStringList m_addresses;
};

#endif // QNETWORKSETTINGSADDRESSMODEL_H
