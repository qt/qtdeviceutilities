/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
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
#ifndef SYSTEMTIME_P_H
#define SYSTEMTIME_P_H

#include "systemtime.h"
#include "timedated_interface.h"

class SystemTimePrivate
{
    Q_DECLARE_PUBLIC(SystemTime)
public:
    SystemTimePrivate(SystemTime *qq)
        :q_ptr(qq)
    {
        m_timeInterface = new OrgFreedesktopTimedate1Interface(QStringLiteral("org.freedesktop.timedate1"),
                                                               QStringLiteral("/org/freedesktop/timedate1"),
                                                               QDBusConnection::systemBus(), qq);

    }

    bool ntp() const {
        if (m_timeInterface)
            return m_timeInterface->nTP();
        return false;
    }

    void setNtp(bool val) {
        if (m_timeInterface)
            m_timeInterface->SetNTP(val, true);
    }

    QString timeZone() const {
        if (m_timeInterface)
            return m_timeInterface->timezone();
        return "";
    }

    void setTimeZone(const QString& aTimeZone) {
        if (m_timeInterface)
            m_timeInterface->SetTimezone(aTimeZone, true);
    }

    void setTime(qlonglong usecsSinceEpoch) {
        if (m_timeInterface)
            m_timeInterface->SetTime(usecsSinceEpoch, false, true);
    }

private:
    OrgFreedesktopTimedate1Interface *m_timeInterface;
    SystemTime *q_ptr;
};


#endif // SYSTEMTIME_P_H
