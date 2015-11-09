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
#ifndef NETWORKCONFIG_H
#define NETWORKCONFIG_H

#include <QObject>
#include <QUrl>
#include <QVariantMap>
#include "networkaddressmodel.h"

class NetworkState : public QObject
{
    Q_OBJECT
    Q_ENUMS(States)
    Q_PROPERTY(States state READ state WRITE setState NOTIFY stateChanged)

public:
    enum States {Idle, Failure, Association, Configuration, Ready, Disconnect, Online, Undefined};

    explicit NetworkState(States aState, QObject *parent = 0)
        : QObject(parent)
    {
        m_state = aState;
    }

    explicit NetworkState(QObject *parent = 0)
        : QObject(parent)
    {
        m_state = Undefined;
    }

    States state() const {
        return m_state;
    }

    void setState(const States aState) {
        m_state = aState;
        emit stateChanged();
    }

signals:
    void stateChanged();

private:
    States m_state;
};

class NetworkType : public QObject
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

    explicit NetworkType(Types aType, QObject *parent = 0)
        : QObject(parent)
    {
        m_type = aType;
    }

    explicit NetworkType(QObject *parent = 0)
        : QObject(parent)
    {
        m_type = Unknown;
    }


    Types type() const {
        return m_type;
    }

    void setType(const Types aType) {
        m_type = aType;
        emit typeChanged();
    }

signals:
    void typeChanged();

private:
    Types m_type;
};

class IPv4Config : public QObject
{
    Q_OBJECT
    Q_ENUMS(MethodType)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString gateway READ gateway WRITE setGateway NOTIFY gatewayChanged)
    Q_PROPERTY(MethodType method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(QString mask READ mask WRITE setMask NOTIFY maskChanged)

public:
    explicit IPv4Config(QObject *parent = 0)
        : QObject(parent)
    {

    }

    enum MethodType {Dhcp, Manual, Off};

    QString address() const {
        return m_address;
    }
    void setAddress(const QString& aAdderess) {
        m_address = aAdderess;
        emit addressChanged();
    }

    QString gateway() const {
        return m_gateway;
    }

    void setGateway(const QString& aGateway) {
        m_gateway = aGateway;
        emit gatewayChanged();
    }

    MethodType method() const {
        return m_method;
    }

    void setMethod(const MethodType aMethod) {
        m_method = aMethod;
        emit methodChanged();
    }

    QString mask() const {
        return m_mask;
    }

    void setMask(const QString& aMask) {
        m_mask = aMask;
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


class IPv6Config : public QObject
{
    Q_OBJECT
    Q_ENUMS(MethodType PrivacyType)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString gateway READ gateway WRITE setGateway NOTIFY gatewayChanged)
    Q_PROPERTY(MethodType method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(PrivacyType privacy READ privacy WRITE setPrivacy NOTIFY privacyChanged)
    Q_PROPERTY(int prefixLength READ prefixLength WRITE setPrefixLength NOTIFY prefixLengthChanged)

public:
    explicit IPv6Config(QObject *parent = 0)
        : QObject(parent)
    {

    }

    enum MethodType {Auto, Manual, Off};
    enum PrivacyType {Disabled, Enabled, Preferred};

    QString address() const {
        return m_address;
    }
    void setAddress(const QString& aAdderess) {
        m_address = aAdderess;
        emit addressChanged();
    }

    QString gateway() const {
        return m_gateway;
    }

    void setGateway(const QString& aGateway) {
        m_gateway = aGateway;
        emit gatewayChanged();
    }

    MethodType method() const {
        return m_method;
    }

    void setMethod(const MethodType aMethod) {
        m_method = aMethod;
        emit methodChanged();
    }

    PrivacyType privacy() const {
        return m_privacy;
    }

    void setPrivacy(const PrivacyType aPrivacy) {
        m_privacy = aPrivacy;
        emit privacyChanged();
    }

    int prefixLength() const {
        return m_prefixLength;
    }

    void setPrefixLength(const int& aPrefixLength) {
        m_prefixLength = aPrefixLength;
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

class ProxyConfig   : public QObject
{
    Q_OBJECT
    Q_ENUMS(MethodType)
public:
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QAbstractItemModel* servers READ servers NOTIFY serversChanged)
    Q_PROPERTY(QAbstractItemModel* excludes READ excludes NOTIFY excludesChanged)
    Q_PROPERTY(MethodType method READ method WRITE setMethod NOTIFY methodChanged)

    explicit ProxyConfig(QObject *parent=NULL)
        : QObject(parent)
    {

    }

    enum MethodType {Direct, Auto, Manual};

    QUrl url() const {
        return m_url;
    }

    void setUrl(const QUrl& aUrl) {
        m_url = aUrl;
        emit urlChanged();
    }

    QAbstractItemModel* servers(void) {
        return &m_servers;
    }

    void setServers(const QStringList& aServers) {
        m_servers.setStringList(aServers);
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

    void setExcludes(const QStringList& aExcludes) {
        m_excludes.setStringList(aExcludes);
        emit excludesChanged();
    }

    MethodType method(void) const {
        return m_method;
    }

    void setMethod(const MethodType& aMethod) {
        m_method = aMethod;
        emit methodChanged();
    }

signals:
    void urlChanged();
    void serversChanged();
    void excludesChanged();
    void methodChanged();
private:
    QUrl m_url;
    NetworkAddressModel m_servers;
    NetworkAddressModel m_excludes;
    MethodType m_method;
};

class WifiConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString bssid READ bssid NOTIFY bssidChanged)
    Q_PROPERTY(int maxRate READ maxRate NOTIFY maxRateChanged)
    Q_PROPERTY(int frequency READ frequency NOTIFY frequencyChanged)
    Q_PROPERTY(QString encryptionMode READ encryptionMode NOTIFY encryptionModeChanged)
    Q_PROPERTY(bool hidden READ hidden NOTIFY hiddenChanged)
public:
    explicit WifiConfig(QObject* parent = 0) :
        QObject(parent) {

    }

    const QString bssid() {
        return m_bssid;
    }

    int maxRate() {
        return m_maxRate;
    }

    int frequency() {
        return m_frequency;
    }

    const QString encryptionMode() {
        return m_encryptionMode;
    }

    bool hidden() const {
        return m_hidden;
    }

signals:
    void bssidChanged(const QString &bssid);
    void maxRateChanged(quint32 rate);
    void frequencyChanged(quint16 frequency);
    void encryptionModeChanged(const QString &mode);
    void hiddenChanged(bool);

private:
    QString m_bssid;
    quint32 m_maxRate;
    quint16 m_frequency;
    QString m_encryptionMode;
    bool m_hidden;
};


#endif
