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
#include "qnetworksettingsuseragent.h"
#include "qnetworksettingsuseragent_p.h"

QT_BEGIN_NAMESPACE

/*!
    \class QNetworkSettingsUserAgent
    \inmodule QtNetworkSettings

    \brief The QNetworkSettingsUserAgent represents the user credentials for
    connecting to a network.

    The QNetworkSettingsService::connectService() method is used to connect to
    a Wifi network. The network name is specified by using a service set
    identifier (SSID). If no password is needed, the connection is created
    immediately. QNetworkSettingsService::state property can be used to
    monitor the state of the connection.

    If a password is needed, the
    QNetworkSettingsUserAgent::showUserCredentialsInput() signal is emitted.
    To create a connection, the password is set by using the
    QNetworkSettingsUserAgent::setPassphrase() method.

    \sa QNetworkSettingsManager
*/

/*!
    \fn QNetworkSettingsUserAgent::showUserCredentialsInput()

    This signal is emitted when user credentials are required for connecting to
    a Wifi network.

    \sa QNetworkSettingsService::connectService()
*/

/*!
    \fn QNetworkSettingsUserAgent::error()

    This signal is emitted when the connection failed due to invalid user
    credentials.
*/

/*!
    \fn QNetworkSettingsUserAgent::ready(bool cancel)

    This signal is emitted when the user has either submitted the password
    for the network, or if \a cancel is \c true, cancelled the password
    query.
*/

/*!
    \fn QNetworkSettingsUserAgent::requestNextConnection()

    This signal is emitted when the network manager attempts to connect using
    the next network interface in the model.
*/

/*!
    Creates a user agent with the parent \a parent.
*/
QNetworkSettingsUserAgent::QNetworkSettingsUserAgent(QObject *parent)
    :QObject(parent)
    ,d_ptr(new QNetworkSettingsUserAgentPrivate(this))
{
}

/*!
    Sets the password for connecting to a Wifi network to \a passphrase.

    This method needs to be called in response to receiving the
    \l showUserCredentialsInput() signal.
*/
void QNetworkSettingsUserAgent::setPassphrase(const QString &passphrase)
{
    Q_D(QNetworkSettingsUserAgent);
    d->setPassphrase(passphrase);
}

/*!
    Cancels the user credentials input request.
*/
void QNetworkSettingsUserAgent::cancelInput()
{
    Q_D(QNetworkSettingsUserAgent);
    d->cancel();
}

/*!
    Returns the password of the user agent.
*/
QString QNetworkSettingsUserAgent::passphrase() const
{
    Q_D(const QNetworkSettingsUserAgent);
    return d->passphrase();
}

/*!
    Sets the name of the network to \a ssid and the password of the user agent
    to \a passphrase.
*/
void QNetworkSettingsUserAgent::setSsidAndPassphrase(const QString &ssid, const QString &passphrase)
{
    Q_D(QNetworkSettingsUserAgent);
    d->setSsidAndPassphrase(ssid, passphrase);
}

/*!
    Clears the connection state.
*/
void QNetworkSettingsUserAgent::clearConnectionState()
{
    Q_D(QNetworkSettingsUserAgent);
    d->clearConnectionState();
}

QT_END_NAMESPACE
