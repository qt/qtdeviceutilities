// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QNETWORKSETTINGSUSERAGENTPRIVATE_H
#define QNETWORKSETTINGSUSERAGENTPRIVATE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/QObject>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusAbstractAdaptor>

QT_BEGIN_NAMESPACE

class QNetworkSettingsUserAgent;
class QNetworkSettingsUserAgentPrivate : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "net.connman.Agent")
    Q_DECLARE_PUBLIC(QNetworkSettingsUserAgent)
public:
    QNetworkSettingsUserAgentPrivate(QNetworkSettingsUserAgent *parent = nullptr);
    void setPassphrase(const QString &passphrase);
    QString passphrase() const {return m_passphrase;}
    void cancel();
    void release();
    void setSsidAndPassphrase(const QString &ssid, const QString &passphrase);
    void clearConnectionState();
public Q_SLOTS: // Dbus methods
    void ReportError(const QDBusObjectPath &path, const QString &param);
    QVariantMap RequestInput(const QDBusObjectPath &path, const QVariantMap &params,
                             const QDBusMessage &message);
    void registerAgent();
private:
    QNetworkSettingsUserAgent *q_ptr;
    QDBusMessage m_pendingMessage;
    bool m_pendingReply;
    QString m_passphrase;
    QString m_ssid;
};

QT_END_NAMESPACE

#endif // QNETWORKSETTINGSUSERAGENTPRIVATE_H

