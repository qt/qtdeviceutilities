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
    if (!m_pendingReply)
        return;

    m_pendingReply = false;
    QDBusMessage errorMessage = m_pendingMessage.createErrorReply(
                              QStringLiteral("net.connman.Agent.Error.Canceled"),
                              QStringLiteral(""));
    QDBusConnection::systemBus().send(errorMessage);
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

    m_pendingReply = true;
    m_pendingMessage = msg;
    emit q->showUserCredentialsInput();
    return QVariantMap();
}

void QNetworkSettingsUserAgentPrivate::setPassphrase(const QString& passphrase)
{
    m_passphrase = passphrase;
    if (!m_pendingReply)
        return;

    QVariantMap response;
    response[PropertyPassphrase] = m_passphrase;

    QDBusMessage reply = m_pendingMessage.createReply();
    reply << response;
    m_pendingReply = false;
    QDBusConnection::systemBus().send(reply);
}
