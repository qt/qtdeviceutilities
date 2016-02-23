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
#ifndef LOCALE_P_H
#define LOCALE_P_H

#include "systemlocale.h"
#include "locale_interface.h"

class SystemLocalePrivate
{
    Q_DECLARE_PUBLIC(SystemLocale)
public:
    SystemLocalePrivate(SystemLocale *qq)
        :q_ptr(qq)
    {
        m_localeInterface = new OrgFreedesktopLocale1Interface(QStringLiteral("org.freedesktop.locale1"),
                                                               QStringLiteral("/org/freedesktop/locale1"),
                                                               QDBusConnection::systemBus(), qq);

        QStringList locale = m_localeInterface->locale();
        foreach (QString l, locale) {
            QStringList nameValue = l.split('=');
            if (nameValue.length() == 2)
                m_localeCache[nameValue.at(0)] = nameValue.at(1);
        }
    }

    QString locale() const {
        return m_localeCache[QStringLiteral("LANG")];
    }

    void setLocale(const QString& aLocale) {
        m_localeCache[QStringLiteral("LANG")] = aLocale;
        updateLocale();
    }

    void updateLocale() {
        QStringList newLocale;
        QMap<QString, QString>::iterator i;
        for (i = m_localeCache.begin(); i != m_localeCache.end(); ++i) {
            QString val = i.key() + "=" + i.value();
            newLocale.append(val);
        }
        m_localeInterface->SetLocale(newLocale, true);
    }


private:
    QMap<QString, QString> m_localeCache;
    OrgFreedesktopLocale1Interface *m_localeInterface;
    SystemLocale *q_ptr;
};

#endif // LOCALE_P_H
