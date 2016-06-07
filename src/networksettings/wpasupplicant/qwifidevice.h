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
#ifndef QWIFIDEVICE_H
#define QWIFIDEVICE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QLoggingCategory>

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(B2QT_WIFI)

class Q_DECL_EXPORT QWifiDevice : public QObject
{
    Q_OBJECT
public:
    explicit QWifiDevice();
    virtual ~QWifiDevice();

    Q_INVOKABLE static bool wifiSupported();
    static QByteArray wifiInterfaceName();
    static void setWifiInterfaceName(const QByteArray &name);
};

QT_END_NAMESPACE

#endif // QWIFIDEVICE_H
