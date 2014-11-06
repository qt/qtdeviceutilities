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
};

QWifiConfigurationPrivate::QWifiConfigurationPrivate(QWifiConfiguration *config)
    : q_ptr(config)
{
}


QWifiConfiguration::QWifiConfiguration(QObject *parent)
    : QObject(parent)
    , d_ptr(new QWifiConfigurationPrivate(this))
{
}

QWifiConfiguration::~QWifiConfiguration()
{
    delete d_ptr;
}

void QWifiConfiguration::setSsid(const QString &ssid)
{
    Q_D(QWifiConfiguration);
    d->m_ssid = ssid;
}

QString QWifiConfiguration::ssid() const
{
    Q_D(const QWifiConfiguration);
    return d->m_ssid;
}

void QWifiConfiguration::setPassphrase(const QString &psk)
{
    Q_D(QWifiConfiguration);
    d->m_psk = psk;
}

QString QWifiConfiguration::passphrase() const
{
    Q_D(const QWifiConfiguration);
    return d->m_psk;
}

void QWifiConfiguration::setProtocol(const QString &protocol)
{
    Q_D(QWifiConfiguration);
    d->m_protocol = protocol;
}

QString QWifiConfiguration::protocol() const
{
    Q_D(const QWifiConfiguration);
    return d->m_protocol;
}

QT_END_NAMESPACE
