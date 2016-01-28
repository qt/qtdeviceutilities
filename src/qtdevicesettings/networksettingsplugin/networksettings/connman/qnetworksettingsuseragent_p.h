/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
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
#ifndef QNETWORKSETTINGSUSERAGENTPRIVATE_H
#define QNETWORKSETTINGSUSERAGENTPRIVATE_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "qnetworksettingsuseragent.h"

QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

const QString AgentPath("/ConnmanAgent");

class QNetworkSettingsUserAgentPrivate : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "net.connman.Agent")
    Q_DECLARE_PUBLIC(QNetworkSettingsUserAgent)
public:
    QNetworkSettingsUserAgentPrivate(QNetworkSettingsUserAgent *parent=0);
    void setUserCredentials(const QString& aUsername, const QString& aPassword);
    void cancel();
    void release();
public Q_SLOTS: // Dbus methods
    void ReportError(const QDBusObjectPath &path, const QString &param);
    QVariantMap RequestInput(const QDBusObjectPath &path, const QVariantMap &params,
                             const QDBusMessage &message);
    void registerAgent();
private:
    QNetworkSettingsUserAgent *q_ptr;
    QDBusMessage m_reply;
    bool m_pendingReply;
};

#endif // QNETWORKSETTINGSUSERAGENTPRIVATE_H
