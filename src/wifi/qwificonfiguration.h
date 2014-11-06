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
#ifndef QWIFICONFIGURATION_H
#define QWIFICONFIGURATION_H

#include <QtCore/QObject>
#include <QtCore/QString>

class QWifiConfigurationPrivate;

QT_BEGIN_NAMESPACE

class Q_DECL_EXPORT QWifiConfiguration : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ssid READ ssid WRITE setSsid)
    Q_PROPERTY(QString passphrase READ passphrase WRITE setPassphrase)
    Q_PROPERTY(QString protocol READ protocol WRITE setProtocol)
public:
    explicit QWifiConfiguration(QObject *parent = 0);
    virtual ~QWifiConfiguration();

    void setSsid(const QString &ssid);
    QString ssid() const;
    void setPassphrase(const QString &psk);
    QString passphrase() const;
    void setProtocol(const QString &protocol);
    QString protocol() const;

private:
    Q_DISABLE_COPY(QWifiConfiguration)
    Q_DECLARE_PRIVATE(QWifiConfiguration)
    QWifiConfigurationPrivate *const d_ptr;
};

QT_END_NAMESPACE

#endif // QWIFICONFIGURATION_H
