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
#include "qnetworksettings.h"

QT_BEGIN_NAMESPACE

QNetworkSettingsState::QNetworkSettingsState(States state, QObject *parent)
    : QObject(parent)
    , m_state(state)
{
}

QNetworkSettingsState::QNetworkSettingsState(QObject *parent)
    : QObject(parent)
    , m_state(Undefined)
{
}

QNetworkSettingsState::States QNetworkSettingsState::state() const {
    return m_state;
}

void QNetworkSettingsState::setState(const States state) {
    m_state = state;
    emit stateChanged();
}

QNetworkSettingsType::QNetworkSettingsType(Types type, QObject *parent)
    : QObject(parent)
    , m_type(type)
{
    m_type = type;
}

QNetworkSettingsType::QNetworkSettingsType(QObject *parent)
    : QObject(parent)
{
    m_type = Unknown;
}

QNetworkSettingsType::Types QNetworkSettingsType::type() const {
    return m_type;
}

void QNetworkSettingsType::setType(const Types type) {
    m_type = type;
    emit typeChanged();
}

QNetworkSettingsIPv4::QNetworkSettingsIPv4(QObject *parent)
    : QObject(parent)
{
}

QString QNetworkSettingsIPv4::address() const {
    return m_address;
}
void QNetworkSettingsIPv4::setAddress(const QString& address) {
    m_address = address;
    emit addressChanged();
}

QString QNetworkSettingsIPv4::gateway() const {
    return m_gateway;
}

void QNetworkSettingsIPv4::setGateway(const QString& gateway) {
    m_gateway = gateway;
    emit gatewayChanged();
}

QNetworkSettingsIPv4::MethodType QNetworkSettingsIPv4::method() const {
    return m_method;
}

void QNetworkSettingsIPv4::setMethod(const MethodType method) {
    m_method = method;
    emit methodChanged();
}

QString QNetworkSettingsIPv4::mask() const {
    return m_mask;
}

void QNetworkSettingsIPv4::setMask(const QString& mask) {
    m_mask = mask;
    emit maskChanged();
}

QNetworkSettingsIPv6::QNetworkSettingsIPv6(QObject *parent)
    : QObject(parent)
{
}

QString QNetworkSettingsIPv6::address() const {
    return m_address;
}

void QNetworkSettingsIPv6::setAddress(const QString& address) {
    m_address = address;
    emit addressChanged();
}

QString QNetworkSettingsIPv6::gateway() const {
    return m_gateway;
}

void QNetworkSettingsIPv6::setGateway(const QString& gateway) {
    m_gateway = gateway;
    emit gatewayChanged();
}

QNetworkSettingsIPv6::MethodType QNetworkSettingsIPv6::method() const {
    return m_method;
}

void QNetworkSettingsIPv6::setMethod(const MethodType method) {
    m_method = method;
    emit methodChanged();
}

QNetworkSettingsIPv6::PrivacyType QNetworkSettingsIPv6::privacy() const {
    return m_privacy;
}

void QNetworkSettingsIPv6::setPrivacy(const PrivacyType privacy) {
    m_privacy = privacy;
    emit privacyChanged();
}

int QNetworkSettingsIPv6::prefixLength() const {
    return m_prefixLength;
}

void QNetworkSettingsIPv6::setPrefixLength(const int& prefixLength) {
    m_prefixLength = prefixLength;
    emit prefixLengthChanged();
}

QNetworkSettingsProxy::QNetworkSettingsProxy(QObject *parent)
    : QObject(parent)
{

}

QUrl QNetworkSettingsProxy::url() const {
    return m_url;
}

void QNetworkSettingsProxy::setUrl(const QUrl& url) {
    m_url = url;
    emit urlChanged();
}

QAbstractItemModel* QNetworkSettingsProxy::servers(void) {
    return &m_servers;
}

void QNetworkSettingsProxy::setServers(const QStringList& servers) {
    m_servers.setStringList(servers);
    emit serversChanged();
}

QStringList QNetworkSettingsProxy::servers() const {
    return m_servers.stringList();
}

QAbstractItemModel* QNetworkSettingsProxy::excludes(void) {
    return &m_excludes;
}

QStringList QNetworkSettingsProxy::excludes() const {
    return m_excludes.stringList();
}

void QNetworkSettingsProxy::setExcludes(const QStringList& excludes) {
    m_excludes.setStringList(excludes);
    emit excludesChanged();
}

QNetworkSettingsProxy::MethodType QNetworkSettingsProxy::method(void) const {
    return m_method;
}

void QNetworkSettingsProxy::setMethod(const MethodType& method) {
    m_method = method;
    emit methodChanged();
}

QNetworkSettingsWireless::QNetworkSettingsWireless(QObject* parent)
  : QObject(parent) {
}

bool QNetworkSettingsWireless::supportsSecurity(SecurityFlags security) {
    if (m_securityFlags & security) {
        return true;
    }
    return false;
}

bool QNetworkSettingsWireless::hidden() const {
    return m_hidden;
}

void QNetworkSettingsWireless::setHidden(const bool hidden) {
    m_hidden = hidden;
    emit hiddenChanged();
}

int QNetworkSettingsWireless::signalStrength() const {
    return m_signalStrength;
}

void QNetworkSettingsWireless::setSignalStrength(const int signalStrength) {
    m_signalStrength = signalStrength;
    emit signalStrengthChanged();
}

void QNetworkSettingsWireless::setSecurity(const SecurityFlags security) {
    if (security == None) {
        m_securityFlags = None;
    } else {
        m_securityFlags |= security;
    }
}

void QNetworkSettingsWireless::setOutOfRange(const bool aOutOfRange) {
    m_isOutOfRange = aOutOfRange;
    emit outOfRangeChanged();
}

bool QNetworkSettingsWireless::outOfRange() const {
    return m_isOutOfRange;
}

QT_END_NAMESPACE
