// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QNETWORKSETTINGSADDRESSMODEL_H
#define QNETWORKSETTINGSADDRESSMODEL_H

#include <QtCore/qstringlistmodel.h>
#include <QtNetworkSettings/qnetworksettingsglobal.h>

QT_BEGIN_NAMESPACE

class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsAddressModel : public QStringListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit QNetworkSettingsAddressModel(QObject *parent = nullptr);
    explicit QNetworkSettingsAddressModel(const QStringList &strings, QObject *parent = nullptr);
    Q_INVOKABLE void append(const QString& address);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void resetChanges();
    void setStringList(const QStringList &addresses);
    int count() const;

Q_SIGNALS:
    void countChanged();
private:
    QStringList m_addresses;
};

QT_END_NAMESPACE

#endif // QNETWORKSETTINGSADDRESSMODEL_H
