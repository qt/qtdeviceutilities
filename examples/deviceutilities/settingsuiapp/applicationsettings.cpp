// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
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

