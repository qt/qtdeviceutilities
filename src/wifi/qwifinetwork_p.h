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
#ifndef QWIFINETWORK_H
#define QWIFINETWORK_H

#include <QtCore/QObject>
#include <QtCore/QString>

QT_BEGIN_NAMESPACE

class QWifiNetwork : public QObject
{
    Q_OBJECT
public:
    explicit QWifiNetwork(QObject *parent = 0);
    virtual ~QWifiNetwork();

    void setBssid(const QString &bssid) { m_bssid = bssid; }
    QString bssid() const { return m_bssid; }
    void setSsid(const QString &ssid);
    QString ssid() const { return m_ssid; }

    void setSignalStrength(int strength);
    int signalStrength() const { return m_signalStrength; }

    void setOutOfRange(bool isOutOfRange);
    // this could be exposed in QWifiManager::Roles,
    // if there is a use case for it, this would require keeping
    // the out of range networks in data model, currently they are
    // removed from the model.
    bool isOutOfRange() const { return m_isOutOfRange; }

    void setFlags(const QString &flags) { m_flags = flags; }
    QString flags() const { return m_flags; }
    bool supportsWPA2() const { return m_flags.contains(QStringLiteral("WPA2")); }
    bool supportsWPA() const { return m_flags.contains(QStringLiteral("WPA")); }
    bool supportsWEP() const { return m_flags.contains(QStringLiteral("WEP")); }
    bool supportsWPS() const { return m_flags.contains(QStringLiteral("WPS")); }

private:
    QString m_bssid;
    QString m_ssid;
    int m_signalStrength;
    QString m_flags;
    bool m_isOutOfRange;
};

QT_END_NAMESPACE

#endif // QWIFINETWORK_H
