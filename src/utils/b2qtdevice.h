/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
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
#ifndef B2QTDEVICE_H
#define B2QTDEVICE_H

#include <qobject.h>

class Q_DECL_EXPORT B2QtDevice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(quint8 displayBrightness READ displayBrightness WRITE setDisplayBrightness NOTIFY displayBrightnessChanged)
    Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)
    Q_PROPERTY(QString ipAddress READ getIPAddress NOTIFY ipAddressChanged)
    Q_PROPERTY(int masterVolume READ masterVolume WRITE setMasterVolume NOTIFY masterVolumeChanged)

public:
    B2QtDevice(QObject *parent = 0);
    ~B2QtDevice();

    quint8 displayBrightness() const;
    QString hostname() const;
    QString getIPAddress() const;
    int masterVolume() const;

    void initAudio();

public Q_SLOTS:
    void reboot();
    void powerOff();

    bool setDisplayBrightness(quint8 value);
    bool setHostname(const QString &name);
    void setMasterVolume(int volume);

signals:
    void displayBrightnessChanged(quint8 newValue);
    void hostnameChanged(const QString &newName);
    void ipAddressChanged(const QString &newAddress);
    void masterVolumeChanged(int newVolume);
};

#endif // B2QTDEVICE_H
