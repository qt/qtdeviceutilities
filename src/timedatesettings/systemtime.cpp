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
#include "systemtime.h"
#include "systemtime_p.h"

SystemTime::SystemTime(QObject *parent) :
    QObject(parent)
    ,d_ptr(new SystemTimePrivate(this))
{
    QTimer *secTimer = new QTimer(this);
    connect(secTimer, &QTimer::timeout, this, &SystemTime::timeChanged);
    secTimer->setInterval(1001);
    secTimer->setSingleShot(false);
    secTimer->start();
}

bool SystemTime::ntp() const
{
    const Q_D(SystemTime);
    return d->ntp();
}

void SystemTime::setNtp(const bool aNtp)
{
   Q_D(SystemTime);
   d->setNtp(aNtp);
}

void SystemTime::setTime(const QDateTime& aTime)
{
    Q_D(SystemTime);
    d->setTime(aTime.toMSecsSinceEpoch()*1000);
    emit timeChanged();
}

QString SystemTime::timeZone() const
{
    Q_D(const SystemTime);
    return d->timeZone();
}

void SystemTime::setTimeZone(const QString& aTimeZone)
{
    Q_D(SystemTime);
    d->setTimeZone(aTimeZone);
    emit timeZoneChanged();
}

QDateTime SystemTime::time() const
{
    return QDateTime::currentDateTime();
}
