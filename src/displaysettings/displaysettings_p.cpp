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
#include <QtMath>
#include <QDirIterator>
#include "displaysettings_p.h"

DisplaySettingsPrivate::DisplaySettingsPrivate(DisplaySettings *qq)
    :q_ptr(qq)
    ,m_brightness(255)
    ,m_lightDevicesInitialized(false)
    ,m_physScreenSize(new PhysicalScreenSize(qq))
{

}

void DisplaySettingsPrivate::initLightDevices()
{
    if (m_lightDevicesInitialized)
        return;
    QDirIterator it(QStringLiteral("/sys/class/backlight"));
    while (it.hasNext()) {
        LightDevice ld;
        ld.deviceFile = it.next() + QStringLiteral("/brightness");
        QFile maxFile(it.filePath() + QStringLiteral("/max_brightness"));
        if (!maxFile.open(QIODevice::ReadOnly))
            continue;
        bool ok = false;
        ld.maxValue = maxFile.read(10).simplified().toUInt(&ok);
        if (!ok || !ld.maxValue)
            continue;
        QFile valFile(ld.deviceFile);
        if (!valFile.open(QIODevice::ReadOnly))
            continue;
        ok = false;
        uint val = valFile.read(10).simplified().toUInt(&ok);
        if (!ok)
            continue;
        // map max->max as that is a common case, otherwise choose a reasonable value
        ld.value = (val == ld.maxValue) ? 255 : (val * 256)/(ld.maxValue+1);
        ld.name = it.fileName();
        m_lightDevices.append(ld);
    }
    if (!m_lightDevices.isEmpty())
        m_brightness = m_lightDevices.at(0).value;
    m_lightDevicesInitialized = true;
}


bool DisplaySettingsPrivate::setDisplayBrightness(int v)
{
    quint8 value = qBound(0, v, 255);
    initLightDevices();
    for (int i = 0; i < m_lightDevices.size(); i++) {
        LightDevice &ld = m_lightDevices[i];
        QFile devFile(ld.deviceFile);
        if (!devFile.open(QIODevice::WriteOnly))
            continue;
        // Maps only 0 to 0, since 0 often means "off"; other values are degrees of "on".
        uint newVal = value ? 1 + ((value * ld.maxValue) / 256) : 0;
        devFile.write(QByteArray::number(newVal));
        ld.value = value;
    }
    m_brightness = value;
    return true;
}

int DisplaySettingsPrivate::displayBrightness()
{
    initLightDevices();
    return m_brightness;
}

int DisplaySettingsPrivate::physicalScreenSizeInch() const
{
    return m_physScreenSize->size();
}

void DisplaySettingsPrivate::setPhysicalScreenSizeInch(int inches)
{
    Q_Q(DisplaySettings);
    if (m_physScreenSize->size() != inches) {
        m_physScreenSize->setSize(inches);
        emit q->physicalScreenSizeInchChanged(inches);
    }
}

bool DisplaySettingsPrivate::physicalScreenSizeOverride() const
{
    return m_physScreenSize->enabled();
}

void DisplaySettingsPrivate::setPhysicalScreenSizeOverride(bool enable)
{
    Q_Q(DisplaySettings);
    if (m_physScreenSize->enabled() != enable) {
        m_physScreenSize->setEnabled(enable);
        emit q->physicalScreenSizeOverrideChanged(enable);
    }
}

PhysicalScreenSize::PhysicalScreenSize(QObject *parent)
    : QObject(parent)
      ,physScreenSizeEnabled(false), physScreenSizeInch(7)
{
    physWriteTimer.setSingleShot(true);
    physWriteTimer.setInterval(1000);
    QObject::connect(&physWriteTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    read(QStringLiteral("/etc/appcontroller.conf"));
    read(QStringLiteral("/var/lib/b2qt/appcontroller.conf.d/physical_screen_size.conf"));
}

PhysicalScreenSize::~PhysicalScreenSize()
{

}

void PhysicalScreenSize::read(const QString &filename)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    int physScreenWidth = 154, physScreenHeight = 90;
    int found = 0;
    while (!f.atEnd()) {
        QByteArray line = f.readLine().trimmed();
        if (line.startsWith(QByteArrayLiteral("env="))) {
            QByteArrayList values = line.split('=');
            if (values.count() == 3) {
                bool ok;
                if (values[1] == QByteArrayLiteral("QT_QPA_EGLFS_PHYSICAL_WIDTH")) {
                    int val = values[2].toInt(&ok);
                    if (ok) {
                        ++found;
                        physScreenWidth = val;
                    }
                } else if (values[1] == QByteArrayLiteral("QT_QPA_EGLFS_PHYSICAL_HEIGHT")) {
                    int val = values[2].toInt(&ok);
                    if (ok) {
                        ++found;
                        physScreenHeight = val;
                    }
                }
            }
        }
    }
    if (found == 2)
        physScreenSizeEnabled = true;

    const qreal diagMM = qSqrt(physScreenWidth * physScreenWidth + physScreenHeight * physScreenHeight);
    physScreenSizeInch = qRound(diagMM / 25.4);
}

void PhysicalScreenSize::onTimeout()
{
    write();
}

void PhysicalScreenSize::write(bool includePhysSize)
{
    QDir(QStringLiteral("/var/lib")).mkpath(QStringLiteral("b2qt/appcontroller.conf.d"));
    write(QStringLiteral("/var/lib/b2qt/appcontroller.conf.d/physical_screen_size.conf"), includePhysSize);
}

void PhysicalScreenSize::write(const QString &filename, bool includePhysSize)
{
    QFile f(filename);

    QByteArrayList lines;
    if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        while (!f.atEnd()) {
            QByteArray line = f.readLine().trimmed();
            if (!line.startsWith(QByteArrayLiteral("env=QT_QPA_EGLFS_PHYSICAL_WIDTH="))
                && !line.startsWith(QByteArrayLiteral("env=QT_QPA_EGLFS_PHYSICAL_HEIGHT=")))
                lines.append(line);
        }
        f.close();
    }

    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
        return;

    const qreal diagMM = physScreenSizeInch * 25.4;
    // Assume 16:9 aspect ratio
    const int physScreenHeight = qRound(diagMM / 1.975);
    const int physScreenWidth = qRound(physScreenHeight * 1.777);

    foreach (const QByteArray &line, lines)
        f.write(line + QByteArrayLiteral("\n"));

    if (includePhysSize)
        f.write(QByteArrayLiteral("env=QT_QPA_EGLFS_PHYSICAL_WIDTH=") + QByteArray::number(physScreenWidth)
                + QByteArrayLiteral("\nenv=QT_QPA_EGLFS_PHYSICAL_HEIGHT=") + QByteArray::number(physScreenHeight)
                + QByteArrayLiteral("\n"));
}

void PhysicalScreenSize::setSize(int inches)
{
    physScreenSizeInch = inches;
    physWriteTimer.start();
}

bool PhysicalScreenSize::enabled() const
{
    return physScreenSizeEnabled;
}

void PhysicalScreenSize::setEnabled(bool enable)
{
    physScreenSizeEnabled = enable;
    // Rewrite appcontroller.conf with or without the physical width/height lines.
    write(enable);
}
