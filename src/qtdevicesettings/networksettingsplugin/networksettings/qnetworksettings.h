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
#ifndef QNETWORKSETTINGS_H
#define QNETWORKSETTINGS_H

#include <QObject>
#include <QUrl>
#include <QVariantMap>
#include "qnetworksettingsaddressmodel.h"

class QNetworkSettingsState : public QObject
{
    Q_OBJECT
    Q_ENUMS(States)
    Q_PROPERTY(States state READ state WRITE setState NOTIFY stateChanged)

public:
    enum States {Idle, Failure, Association, Configuration, Ready, Disconnect, Online, Undefined};

    explicit QNetworkSettingsState(States state, QObject *parent = 0)
        : QObject(parent)
    {
        m_state = state;
    }

    explicit QNetworkSettingsState(QObject *parent = 0)
        : QObject(parent)
    {
        m_state = Undefined;
    }

    States state() const {
        return m_state;
    }

    void setState(const States state) {
        m_state = state;
        emit stateChanged();
    }

signals:
    void stateChanged();

private:
    States m_state;
};

class QNetworkSettingsType : public QObject
{
    Q_OBJECT
    Q_ENUMS(Types)
    Q_PROPERTY(Types type READ type WRITE setType NOTIFY typeChanged)

public:
    enum Types {
        Wired,
        Wifi,
        Bluetooth,
        Unknown
    };

    explicit QNetworkSettingsType(Types type, QObject *parent = 0)
        : QObject(parent)
    {
        m_type = type;
    }

    explicit QNetworkSettingsType(QObject *parent = 0)
        : QObject(parent)
    {
        m_type = Unknown;
    }


    Types type() const {
        return m_type;
    }

    void setType(const Types type) {
        m_type = type;
        emit typeChanged();
    }

signals:
    void typeChanged();

private:
    Types m_type;
};

class QNetworkSettingsIPv4 : public QObject
{
    Q_OBJECT
    Q_ENUMS(MethodType)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString gateway READ gateway WRITE setGateway NOTIFY gatewayChanged)
    Q_PROPERTY(MethodType method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(QString mask READ mask WRITE setMask NOTIFY maskChanged)

public:
    explicit QNetworkSettingsIPv4(QObject *parent = 0)
        : QObject(parent)
    {

    }

    enum MethodType {Dhcp, Manual, Off};

    QString address() const {
        return m_address;
    }
    void setAddress(const QString& adderess) {
        m_address = adderess;
        emit addressChanged();
    }

    QString gateway() const {
        return m_gateway;
    }

    void setGateway(const QString& gateway) {
        m_gateway = gateway;
        emit gatewayChanged();
    }

    MethodType method() const {
        return m_method;
    }

    void setMethod(const MethodType method) {
        m_method = method;
        emit methodChanged();
    }

    QString mask() const {
        return m_mask;
    }

    void setMask(const QString& mask) {
        m_mask = mask;
        emit maskChanged();
    }

signals:
    void addressChanged();
    void gatewayChanged();
    void methodChanged();
    void maskChanged();

private:
    QString m_address;
    QString m_gateway;
    MethodType m_method;
    QString m_mask;
};


class QNetworkSettingsIPv6 : public QObject
{
    Q_OBJECT
    Q_ENUMS(MethodType PrivacyType)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString gateway READ gateway WRITE setGateway NOTIFY gatewayChanged)
    Q_PROPERTY(MethodType method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(PrivacyType privacy READ privacy WRITE setPrivacy NOTIFY privacyChanged)
    Q_PROPERTY(int prefixLength READ prefixLength WRITE setPrefixLength NOTIFY prefixLengthChanged)

public:
    explicit QNetworkSettingsIPv6(QObject *parent = 0)
        : QObject(parent)
    {

    }

    enum MethodType {Auto, Manual, Off};
    enum PrivacyType {Disabled, Enabled, Preferred};

    QString address() const {
        return m_address;
    }
    void setAddress(const QString& adderess) {
        m_address = adderess;
        emit addressChanged();
    }

    QString gateway() const {
        return m_gateway;
    }

    void setGateway(const QString& gateway) {
        m_gateway = gateway;
        emit gatewayChanged();
    }

    MethodType method() const {
        return m_method;
    }

    void setMethod(const MethodType method) {
        m_method = method;
        emit methodChanged();
    }

    PrivacyType privacy() const {
        return m_privacy;
    }

    void setPrivacy(const PrivacyType privacy) {
        m_privacy = privacy;
        emit privacyChanged();
    }

    int prefixLength() const {
        return m_prefixLength;
    }

    void setPrefixLength(const int& prefixLength) {
        m_prefixLength = prefixLength;
        emit prefixLengthChanged();
    }

signals:
    void addressChanged();
    void gatewayChanged();
    void methodChanged();
    void privacyChanged();
    void prefixLengthChanged();

private:
    QString m_address;
    QString m_gateway;
    MethodType m_method;
    PrivacyType m_privacy;
    int m_prefixLength;
};

class QNetworkSettingsProxy   : public QObject
{
    Q_OBJECT
    Q_ENUMS(MethodType)
public:
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QAbstractItemModel* servers READ servers NOTIFY serversChanged)
    Q_PROPERTY(QAbstractItemModel* excludes READ excludes NOTIFY excludesChanged)
    Q_PROPERTY(MethodType method READ method WRITE setMethod NOTIFY methodChanged)

    explicit QNetworkSettingsProxy(QObject *parent=NULL)
        : QObject(parent)
    {

    }

    enum MethodType {Direct, Auto, Manual};

    QUrl url() const {
        return m_url;
    }

    void setUrl(const QUrl& url) {
        m_url = url;
        emit urlChanged();
    }

    QAbstractItemModel* servers(void) {
        return &m_servers;
    }

    void setServers(const QStringList& servers) {
        m_servers.setStringList(servers);
        emit serversChanged();
    }

    QStringList servers() const {
        return m_servers.stringList();
    }

    QAbstractItemModel* excludes(void) {
        return &m_excludes;
    }

    QStringList excludes() const {
        return m_excludes.stringList();
    }

    void setExcludes(const QStringList& excludes) {
        m_excludes.setStringList(excludes);
        emit excludesChanged();
    }

    MethodType method(void) const {
        return m_method;
    }

    void setMethod(const MethodType& method) {
        m_method = method;
        emit methodChanged();
    }

signals:
    void urlChanged();
    void serversChanged();
    void excludesChanged();
    void methodChanged();
private:
    QUrl m_url;
    QNetworkSettingsAddressModel m_servers;
    QNetworkSettingsAddressModel m_excludes;
    MethodType m_method;
};

class QNetworkSettingsWireless : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int signalStrength READ signalStrength WRITE setSignalStrength NOTIFY signalStrengthChanged)
    Q_PROPERTY(bool hidden READ hidden NOTIFY hiddenChanged)
    Q_PROPERTY(bool isOutOfRange READ outOfRange WRITE setOutOfRange NOTIFY outOfRangeChanged)
public:
    explicit QNetworkSettingsWireless(QObject* parent = 0) :
        QObject(parent) {
    }

    enum SecurityFlags {None=1, WEP=2, WPA=4, WPA2=8};

    Q_INVOKABLE bool supportsSecurity(SecurityFlags security) {
        if (m_securityFlags & security) {
            return true;
        }
        return false;
    }

    bool hidden() const {
        return m_hidden;
    }

    void setHidden(const bool hidden) {
        m_hidden = hidden;
        emit hiddenChanged();
    }

    int signalStrength() const {
        return m_signalStrength;
    }

    void setSignalStrength(const int signalStrength) {
        m_signalStrength = signalStrength;
        emit signalStrengthChanged();
    }

    void setSecurity(const SecurityFlags security) {
        if (security == None) {
            m_securityFlags = None;
        }
        else {
            m_securityFlags |= security;
        }
    }

    void setOutOfRange(const bool aOutOfRange) {
        m_isOutOfRange = aOutOfRange;
        emit outOfRangeChanged();
    }

    bool outOfRange() const {
        return m_isOutOfRange;
    }

signals:
    void hiddenChanged();
    void signalStrengthChanged();
    void passwordChanged();
    void outOfRangeChanged();
private:
    quint16 m_securityFlags;
    bool m_hidden;
    int m_signalStrength;
    bool m_isOutOfRange;
};
#endif //QNETWORKSETTINGS_H
