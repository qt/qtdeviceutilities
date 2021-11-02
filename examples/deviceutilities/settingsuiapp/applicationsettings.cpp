/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt for Device Creation.
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
#include "applicationsettings.h"
#include <QGuiApplication>
#include <QScreen>

ApplicationSettings::ApplicationSettings(QObject *parent)
    : QObject(parent)
{
    m_screen = qobject_cast<QGuiApplication *>(QCoreApplication::instance())->primaryScreen();

    if (m_screen->orientation() == Qt::PortraitOrientation) {
        m_width = m_screen->availableGeometry().height();
        m_height = m_screen->availableGeometry().width();
    } else {
        m_width = m_screen->availableGeometry().width();
        m_height = m_screen->availableGeometry().height();
    }
}

bool ApplicationSettings::isHighDpi()
{
    return (m_screen->devicePixelRatio() >= 2.0);
}

int ApplicationSettings::width()
{
    return m_width;
}

int ApplicationSettings::height()
{
    return m_height;
}

