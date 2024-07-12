// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QNETWORKSETTINGSINTERFACE_H
#define QNETWORKSETTINGSINTERFACE_H

#include <QtCore/qobject.h>
#include <QtNetworkSettings/qnetworksettings.h>

QT_BEGIN_NAMESPACE

class QNetworkSettingsInterfacePrivate;
class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsInterface : public QObject
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
