// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

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
