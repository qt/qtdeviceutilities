/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef DISPLAYSETTINGSPRIVATE_H
#define DISPLAYSETTINGSPRIVATE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QTimer>
#include "displaysettings.h"

class PhysicalScreenSize : public QObject
{
    Q_OBJECT

public:
    explicit PhysicalScreenSize(QObject *parent = Q_NULLPTR);
    virtual ~PhysicalScreenSize();

    void setSize(int inches);
    void setSizeMm(int widthMm, int heightMm);
    int size() const { return physScreenSizeInch; }
    int widthMm() const { return physScreenWidthMm; }
    int heightMm() const { return physScreenHeightMm; }
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
    int physScreenWidthMm;
    int physScreenHeightMm;
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
    int physicalScreenWidthMm() const;
    int physicalScreenHeightMm() const;
    bool physicalScreenSizeOverride() const;

    bool setDisplayBrightness(int value);
    void setPhysicalScreenSizeInch(int inches);
    void setPhysicalScreenWidthMm(int newWidth);
    void setPhysicalScreenHeightMm(int newHeight);
    void setPhysicalScreenSizeOverride(bool enable);
private:
    void initLightDevices();
    DisplaySettings *q_ptr;
    int m_brightness;
    int m_screenSizeInch;
    int m_screenWidthMm;
    int m_screenHeightMm;
    bool m_sizeOverride;
    QList<LightDevice> m_lightDevices;
    bool m_lightDevicesInitialized;
    PhysicalScreenSize *m_physScreenSize;
};

#endif // DISPLAYSETTINGSPRIVATE_H
