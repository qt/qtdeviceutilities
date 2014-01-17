/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://qt.digia.com/
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://qt.digia.com/
**
****************************************************************************/
#ifndef QWIFINETWORK_H
#define QWIFINETWORK_H

#include <QtCore/QByteArray>
#include <QtCore/QObject>

class QWifiManager;

class QWifiNetwork : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QByteArray bssid READ bssid CONSTANT)
    Q_PROPERTY(QByteArray ssid READ ssid CONSTANT)
    Q_PROPERTY(int signalStrength READ signalStrength NOTIFY signalStrengthChanged)
    Q_PROPERTY(bool supportsWPA2 READ supportsWPA2 CONSTANT)
    Q_PROPERTY(bool supportsWPA READ supportsWPA CONSTANT)
    Q_PROPERTY(bool supportsWEP READ supportsWEP CONSTANT)
    Q_PROPERTY(bool supportsWPS READ supportsWPS CONSTANT)

public:
    QWifiNetwork();

    QByteArray bssid() const { return m_bssid; }
    void setBssid(const QByteArray &id) { m_bssid = id; }

    QByteArray ssid() const { return m_ssid; }
    void setSsid(const QByteArray &id) { m_ssid = id; }

    int signalStrength() const { return m_signalStrength; }
    void setSignalStrength(int strength);

    QByteArray flags() const { return m_flags; }
    void setFlags(const QByteArray &f) { m_flags = f; }
    bool supportsWPA2() const { return m_flags.contains("WPA2"); }
    bool supportsWPA() const { return m_flags.contains("WPA"); }
    bool supportsWEP() const { return m_flags.contains("WEP"); }
    bool supportsWPS() const { return m_flags.contains("WPS"); }

signals:
    void signalStrengthChanged(int arg);

private:
    QByteArray m_bssid;
    QByteArray m_ssid;
    int m_signalStrength;

    QByteArray m_flags;
};

#endif // QWIFINETWORK_H
