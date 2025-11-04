// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include <QtDBus/QDBusConnection>

#include "qnetworksettingsuseragent.h"
#include "qnetworksettingsuseragent_p.h"
#include "connmancommon.h"

QT_BEGIN_NAMESPACE

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
    if (!m_ssid.isEmpty())
        q->requestNextConnection();
    else
        emit q->error();
}

void QNetworkSettingsUserAgentPrivate::registerAgent()
{
    Q_Q(QNetworkSettingsUserAgent);
    QDBusConnection::systemBus().registerObject(PropertyAgentPath, q);
}

QVariantMap QNetworkSettingsUserAgentPrivate::RequestInput(const QDBusObjectPath &path, const QVariantMap &params, const QDBusMessage &msg)
{
    Q_Q(QNetworkSettingsUserAgent);
    Q_UNUSED(path);
    QVariant name = params[PropertyName];
    if (!name.isValid()) {
        m_ssid.clear();
    }
    QVariantMap response;
    QVariant passPhrase = params[PropertyPassphrase];
    if (name.isValid() && !m_ssid.isEmpty()) {
        response[PropertyName] = m_ssid;
    }
    if (passPhrase.isValid()) {
        if (!m_passphrase.isEmpty()) {
            response[PropertyPassphrase] = m_passphrase;
        } else {
            msg.setDelayedReply(true);
            m_pendingMessage = msg;
            m_pendingReply = true;
            emit q->showUserCredentialsInput();
            return QVariantMap();
        }
    }
    return response;
}

void QNetworkSettingsUserAgentPrivate::setPassphrase(const QString& passphrase)
{
    m_passphrase = passphrase;
    if (m_pendingReply) {
        QVariantMap response;
        if (!m_ssid.isEmpty()) {
            response[PropertyName] = m_ssid;
        }
        response[PropertyPassphrase] = m_passphrase;
        QDBusMessage reply = m_pendingMessage.createReply();
        reply << response;
        m_pendingReply = false;
        QDBusConnection::systemBus().send(reply);
    }
}

void QNetworkSettingsUserAgentPrivate::setSsidAndPassphrase(const QString &ssid, const QString &passphrase)
{
    m_ssid = ssid;
    m_passphrase = passphrase;
}

void QNetworkSettingsUserAgentPrivate::clearConnectionState()
{
    m_passphrase.clear();
    m_ssid.clear();
}

QT_END_NAMESPACE
