/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
******************************************************************************/
#ifndef QNETWORKSETTINGSADDRESSMODEL_H
#define QNETWORKSETTINGSADDRESSMODEL_H

#include <QStringListModel>

QT_BEGIN_NAMESPACE

class Q_DECL_EXPORT QNetworkSettingsAddressModel : public QStringListModel
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
