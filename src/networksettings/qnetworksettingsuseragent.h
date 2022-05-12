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
#ifndef QNETWORKSETTINGSUSERAGENT_H
#define QNETWORKSETTINGSUSERAGENT_H

#include <QObject>

QT_BEGIN_NAMESPACE

class QNetworkSettingsUserAgentPrivate;
class Q_DECL_EXPORT QNetworkSettingsUserAgent : public QObject
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
