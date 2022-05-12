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
#ifndef QNETWORKSETTINGSINTERFACE_H
#define QNETWORKSETTINGSINTERFACE_H

#include <QObject>
#include <QtNetworkSettings/qnetworksettings.h>

QT_BEGIN_NAMESPACE

class QNetworkSettingsInterfacePrivate;
class Q_DECL_EXPORT QNetworkSettingsInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QNetworkSettingsState::State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QNetworkSettingsType::Type type READ type NOTIFY typeChanged)
    Q_PROPERTY(bool powered READ powered WRITE setPowered NOTIFY poweredChanged)

public:
    explicit QNetworkSettingsInterface(QObject* parent = nullptr);
    QString name() const;
    QNetworkSettingsState::State state();
    QNetworkSettingsType::Type type();
    bool powered() const;
    void setPowered(const bool powered);
    Q_INVOKABLE void scanServices();

Q_SIGNALS:
    void nameChanged();
    void stateChanged();
    void typeChanged();
    void poweredChanged();

protected:
    friend class NetworksInterfaceModel;
    QNetworkSettingsInterfacePrivate *d_ptr;

    Q_DISABLE_COPY(QNetworkSettingsInterface)
    Q_DECLARE_PRIVATE(QNetworkSettingsInterface)
};

QT_END_NAMESPACE

#endif // QNETWORKSETTINGSINTERFACE_H
