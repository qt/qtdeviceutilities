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
#ifndef SYSTEMTIME_P_H
#define SYSTEMTIME_P_H

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

#include "systemtime.h"
#include "timedated_interface.h"

QT_BEGIN_NAMESPACE

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
            m_timeInterface->SetNTP(val, true).waitForFinished();
    }

    QString timeZone() const {
        if (m_timeInterface)
            return m_timeInterface->timezone();
        return QString();
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

QT_END_NAMESPACE

#endif // SYSTEMTIME_P_H
