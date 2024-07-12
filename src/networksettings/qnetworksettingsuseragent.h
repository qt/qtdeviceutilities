// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QNETWORKSETTINGSUSERAGENT_H
#define QNETWORKSETTINGSUSERAGENT_H

#include <QtCore/qobject.h>
#include <QtNetworkSettings/qnetworksettingsglobal.h>

QT_BEGIN_NAMESPACE

class QNetworkSettingsUserAgentPrivate;
class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsUserAgent : public QObject
{
    Q_OBJECT
public:
    explicit QNetworkSettingsUserAgent(QObject *parent = nullptr);
    Q_INVOKABLE void setPassphrase(const QString &passphrase);
    Q_INVOKABLE void cancelInput();
    QString passphrase() const;
    void setSsidAndPassphrase(const QString &ssid, const QString &passphrase);
    void clearConnectionState();

Q_SIGNALS:
    void showUserCredentialsInput();
    void error();
    void ready(bool cancel);
    void requestNextConnection();
private:
    QNetworkSettingsUserAgentPrivate *d_ptr;

    Q_DISABLE_COPY(QNetworkSettingsUserAgent)
    Q_DECLARE_PRIVATE(QNetworkSettingsUserAgent)
};

QT_END_NAMESPACE

#endif // QNETWORKSETTINGSUSERAGENT_H
