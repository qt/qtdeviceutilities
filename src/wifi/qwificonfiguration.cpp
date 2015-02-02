/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://www.qt.io
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://www.qt.io
**
****************************************************************************/
#include "qwificonfiguration.h"

QT_BEGIN_NAMESPACE

class QWifiConfigurationPrivate
{
    Q_DECLARE_PUBLIC(QWifiConfiguration)
public:
    QWifiConfigurationPrivate(QWifiConfiguration *config);

    // member variables
    QWifiConfiguration *const q_ptr;
    QString m_ssid;
    QString m_psk;
    QString m_protocol;
    bool m_ssidHidden;
};

QWifiConfigurationPrivate::QWifiConfigurationPrivate(QWifiConfiguration *config)
    : q_ptr(config)
    , m_ssidHidden(false)
{
}

/*!
    \class QWifiConfiguration
    \inmodule B2Qt.Wifi.Cpp
    \ingroup wifi-cppclasses
    \brief Used to define a network configuration.

    QWifiConfiguration object represents a single network configuration. Use it
    to configure properties of your network. For example, passphrase, security
    protocol to use, and so on. QWifiManager::connect() function uses this
    information to find a network that matches the provided configuration, before
    establishing a connection.
 */

/*!
    Constructs a configuration object with parent \a parent.
*/
QWifiConfiguration::QWifiConfiguration(QObject *parent)
    : QObject(parent)
    , d_ptr(new QWifiConfigurationPrivate(this))
{
}

/*!
    Destroys the configuration object.
*/
QWifiConfiguration::~QWifiConfiguration()
{
    delete d_ptr;
}

/*!
    \property QWifiConfiguration::ssid
    \brief a human-readable name of a Wifi network
*/
QString QWifiConfiguration::ssid() const
{
    Q_D(const QWifiConfiguration);
    return d->m_ssid;
}

void QWifiConfiguration::setSsid(const QString &ssid)
{
    Q_D(QWifiConfiguration);
    d->m_ssid = ssid;
}

/*!
    \property QWifiConfiguration::passphrase
    \brief a passphrase to use for authenticating access to a network
*/
QString QWifiConfiguration::passphrase() const
{
    Q_D(const QWifiConfiguration);
    return d->m_psk;
}

void QWifiConfiguration::setPassphrase(const QString &passphrase)
{
    Q_D(QWifiConfiguration);
    d->m_psk = passphrase;
}

/*!
    \property QWifiConfiguration::protocol
    \brief a security protocol to use for Wifi connection

    WPA is used by default if protocol is not explicitly set.
    Supported values are: WPA, WPA2, WEP, WPS.
*/
QString QWifiConfiguration::protocol() const
{
    Q_D(const QWifiConfiguration);
    return d->m_protocol;
}

void QWifiConfiguration::setProtocol(const QString &protocol)
{
    Q_D(QWifiConfiguration);
    d->m_protocol = protocol;
}

/*!
    \property QWifiConfiguration::ssidHidden
    \brief Holds whether a Wifi access point broadcasts its SSID

    If a Wifi access point does not broadcast its SSID, setting this
    property to \c true ensures that the Wifi backend can detect the
    specified network.

    By default this property is set to \c false.
*/
bool QWifiConfiguration::isSsidHidden() const
{
    Q_D(const QWifiConfiguration);
    return d->m_ssidHidden;
}

void QWifiConfiguration::setSsidHidden(bool hidden)
{
    Q_D(QWifiConfiguration);
    d->m_ssidHidden = hidden;
}

QT_END_NAMESPACE
