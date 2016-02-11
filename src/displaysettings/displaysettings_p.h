/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef DISPLAYSETTINGSPRIVATE_H
#define DISPLAYSETTINGSPRIVATE_H

#include <QTimer>
#include "displaysettings.h"

class PhysicalScreenSize : public QObject
{
    Q_OBJECT

public:
    explicit PhysicalScreenSize(QObject *parent=0);
    virtual ~PhysicalScreenSize();

    void setSize(int inches);
    int size() const { return physScreenSizeInch; }
    bool enabled() const;
    void setEnabled(bool enable);

private slots:
    void onTimeout();

private:
    void read(const QString &filename);
    void write(bool includePhysSize = true);
    void write(const QString &filename, bool includePhysSize = true);

    bool physScreenSizeEnabled;
    int physScreenSizeInch;
    QTimer physWriteTimer;
};

class LightDevice
{
public:
    QString name;
    QString deviceFile;
    quint8 value;
    uint maxValue;
};

class DisplaySettingsPrivate
{
    Q_DECLARE_PUBLIC(DisplaySettings)
public:
    DisplaySettingsPrivate(DisplaySettings* qq);
    int displayBrightness();
    int physicalScreenSizeInch() const;
    bool physicalScreenSizeOverride() const;

    bool setDisplayBrightness(int value);
    void setPhysicalScreenSizeInch(int inches);
    void setPhysicalScreenSizeOverride(bool enable);
private:
    void initLightDevices();
    DisplaySettings *q_ptr;
    int m_brightness;
    int m_screenSizeInch;
    bool m_sizeOverride;
    QList<LightDevice> m_lightDevices;
    bool m_lightDevicesInitialized;
    PhysicalScreenSize *m_physScreenSize;
};

#endif // DISPLAYSETTINGSPRIVATE_H
