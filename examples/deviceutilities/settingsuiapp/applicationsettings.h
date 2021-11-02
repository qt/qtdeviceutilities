/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
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

#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QObject>
#include <QQmlEngine>

QT_FORWARD_DECLARE_CLASS(QScreen)

class ApplicationSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int width READ width CONSTANT FINAL)
    Q_PROPERTY(int height READ height CONSTANT FINAL)
    Q_PROPERTY(bool isHighDpi READ isHighDpi NOTIFY isHighDpiChanged FINAL)

    QML_ELEMENT
    QML_SINGLETON

public:
    explicit ApplicationSettings(QObject *parent = nullptr);

    int width();
    int height();
    bool isHighDpi();

signals:
    void isHighDpiChanged(bool isHighDpi);

private:
    QScreen *m_screen = nullptr;
    int m_width = 1280;
    int m_height = 720;
};

#endif // APPLICATIONSETTINGS_H
