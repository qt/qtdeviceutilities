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

#ifndef QNETWORKSETTINGSINTERFACE_H
#define QNETWORKSETTINGSINTERFACE_H

#include <QObject>
#include "qnetworksettings.h"

QT_FORWARD_DECLARE_CLASS(QNetworkSettingsInterfacePrivate)

class QNetworkSettingsInterface : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QNetworkSettingsState::States state READ state NOTIFY stateChanged)
    Q_PROPERTY(QNetworkSettingsType::Types type READ type NOTIFY typeChanged)
    Q_PROPERTY(bool powered READ powered WRITE setPowered NOTIFY poweredChanged)

public:
    explicit QNetworkSettingsInterface(QObject* parent=0);
    QString name() const;
    QNetworkSettingsState::States state();
    QNetworkSettingsType::Types type();
    bool powered() const;
    void setPowered(const bool powered);
    Q_INVOKABLE void scanServices();

signals:
    void nameChanged();
    void stateChanged();
    void typeChanged();
    void poweredChanged();

protected:
    friend class NetworksInterfaceModel;
    QNetworkSettingsInterfacePrivate *d_ptr;

    Q_DISABLE_COPY(QNetworkSettingsInterface)
    Q_DECLARE_PRIVATE(QNetworkSettingsInterface)
};

#endif // QNETWORKSETTINGSINTERFACE_H
