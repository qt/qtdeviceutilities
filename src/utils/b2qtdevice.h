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
#ifndef B2QTDEVICE_H
#define B2QTDEVICE_H

#include <qobject.h>

class Q_DECL_EXPORT B2QtDevice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int displayBrightness READ displayBrightness WRITE setDisplayBrightness NOTIFY displayBrightnessChanged)
    Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)
    Q_PROPERTY(QString ipAddress READ getIPAddress NOTIFY ipAddressChanged)
    Q_PROPERTY(int masterVolume READ masterVolume WRITE setMasterVolume NOTIFY masterVolumeChanged)
    Q_PROPERTY(int physicalScreenSizeInch READ physicalScreenSizeInch WRITE setPhysicalScreenSizeInch NOTIFY physicalScreenSizeInchChanged)
    Q_PROPERTY(bool physicalScreenSizeOverride READ physicalScreenSizeOverride WRITE setPhysicalScreenSizeOverride NOTIFY physicalScreenSizeOverrideChanged)

public:
    B2QtDevice(QObject *parent = 0);
    ~B2QtDevice();

    int displayBrightness() const;
    QString hostname() const;
    QString getIPAddress() const;
    int masterVolume() const;
    int physicalScreenSizeInch() const;
    bool physicalScreenSizeOverride() const;

    void initAudio();

public Q_SLOTS:
    void reboot();
    void powerOff();

    bool setDisplayBrightness(int value);
    bool setHostname(const QString &name);
    void setMasterVolume(int volume);
    void setPhysicalScreenSizeInch(int inches);
    void setPhysicalScreenSizeOverride(bool enable);

signals:
    void displayBrightnessChanged(int newValue);
    void hostnameChanged(const QString &newName);
    void ipAddressChanged(const QString &newAddress);
    void masterVolumeChanged(int newVolume);
    void physicalScreenSizeInchChanged(int newInches);
    void physicalScreenSizeOverrideChanged(bool newValue);
};

#endif // B2QTDEVICE_H
