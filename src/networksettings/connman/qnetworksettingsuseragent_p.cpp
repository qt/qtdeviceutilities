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
#include "qnetworksettingsuseragent_p.h"
#include "connmancommon.h"

const QString PropertyPassphrase(QStringLiteral("Passphrase"));

QNetworkSettingsUserAgentPrivate::QNetworkSettingsUserAgentPrivate(QNetworkSettingsUserAgent *parent)
    :QDBusAbstractAdaptor(parent)
    ,q_ptr(parent)
    ,m_pendingReply(false)
{

    setAutoRelaySignals(true);
    QMetaObject::invokeMethod(this, "registerAgent", Qt::QueuedConnection);
}

void QNetworkSettingsUserAgentPrivate::cancel()
{
    // handle method call net.connman.Agent.Cancel
    m_pendingReply = false;
    QMetaObject::invokeMethod(parent(), "Cancel");
}

void QNetworkSettingsUserAgentPrivate::release()
{
    // handle method call net.connman.Agent.Release
    QMetaObject::invokeMethod(parent(), "Release");
}

void QNetworkSettingsUserAgentPrivate::ReportError(const QDBusObjectPath &path, const QString &param)
{
    Q_Q(QNetworkSettingsUserAgent);
    Q_UNUSED(path);
    Q_UNUSED(param);
    emit q->error();
}

void QNetworkSettingsUserAgentPrivate::registerAgent()
{
    Q_Q(QNetworkSettingsUserAgent);
    QDBusConnection::systemBus().registerObject(QStringLiteral(AgentPath), q);
}

QVariantMap QNetworkSettingsUserAgentPrivate::RequestInput(const QDBusObjectPath &path, const QVariantMap &params, const QDBusMessage &msg)
{
    Q_Q(QNetworkSettingsUserAgent);
    Q_UNUSED(path);
    Q_UNUSED(params);
    msg.setDelayedReply(true);
    m_reply = msg.createReply();
    m_pendingReply = true;
    emit q->showUserCredentialsInput();
    return QVariantMap();
}

void QNetworkSettingsUserAgentPrivate::setUserCredentials(const QString& username,
                                                          const QString& password)
{
    m_username = username;
    m_passphrase = password;
    QVariantMap response;
    response[PropertyName] = m_username;
    response[PropertyPassphrase] = m_passphrase;
    m_reply << response;
    m_pendingReply = false;
    QDBusConnection::systemBus().send(m_reply);
}
