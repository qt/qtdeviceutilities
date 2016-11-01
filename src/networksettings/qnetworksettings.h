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
#ifndef QNETWORKSETTINGS_H
#define QNETWORKSETTINGS_H

#include <QObject>
#include <QUrl>
#include <QVariantMap>
#include "qnetworksettingsaddressmodel.h"

class Q_DECL_EXPORT QNetworkSettingsState : public QObject
{
    Q_OBJECT
    Q_ENUMS(States)
    Q_PROPERTY(States state READ state WRITE setState NOTIFY stateChanged)

public:
    enum States {Idle, Failure, Association, Configuration, Ready, Disconnect, Online, Undefined};

    explicit QNetworkSettingsState(States state, QObject *parent = Q_NULLPTR);
    explicit QNetworkSettingsState(QObject *parent = Q_NULLPTR);

    States state() const;
    void setState(const States state);

Q_SIGNALS:
    void stateChanged();

private:
    States m_state;
};

class Q_DECL_EXPORT QNetworkSettingsType : public QObject
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

    explicit QNetworkSettingsType(Types type, QObject *parent = Q_NULLPTR);
    explicit QNetworkSettingsType(QObject *parent = Q_NULLPTR);

    Types type() const;
    void setType(const Types type);

Q_SIGNALS:
    void typeChanged();

private:
    Types m_type;
};

class Q_DECL_EXPORT QNetworkSettingsIPv4 : public QObject
{
    Q_OBJECT
    Q_ENUMS(MethodType)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString gateway READ gateway WRITE setGateway NOTIFY gatewayChanged)
    Q_PROPERTY(MethodType method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(QString mask READ mask WRITE setMask NOTIFY maskChanged)

public:
    explicit QNetworkSettingsIPv4(QObject *parent = Q_NULLPTR);

    enum MethodType {Dhcp, Manual, Off};

    QString address() const;
    void setAddress(const QString& address);
    QString gateway() const;
    void setGateway(const QString& gateway);
    MethodType method() const;
    void setMethod(const MethodType method);
    QString mask() const;
    void setMask(const QString& mask);

Q_SIGNALS:
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


class Q_DECL_EXPORT QNetworkSettingsIPv6 : public QObject
{
    Q_OBJECT
    Q_ENUMS(MethodType PrivacyType)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString gateway READ gateway WRITE setGateway NOTIFY gatewayChanged)
    Q_PROPERTY(MethodType method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(PrivacyType privacy READ privacy WRITE setPrivacy NOTIFY privacyChanged)
    Q_PROPERTY(int prefixLength READ prefixLength WRITE setPrefixLength NOTIFY prefixLengthChanged)

public:
    explicit QNetworkSettingsIPv6(QObject *parent = Q_NULLPTR);

    enum MethodType {Auto, Manual, Off};
    enum PrivacyType {Disabled, Enabled, Preferred};

    QString address() const;
    void setAddress(const QString& address);
    QString gateway() const;
    void setGateway(const QString& gateway);
    MethodType method() const;
    void setMethod(const MethodType method);
    PrivacyType privacy() const;
    void setPrivacy(const PrivacyType privacy);
    int prefixLength() const;
    void setPrefixLength(const int& prefixLength);

Q_SIGNALS:
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

class Q_DECL_EXPORT QNetworkSettingsProxy   : public QObject
{
    Q_OBJECT
    Q_ENUMS(MethodType)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QAbstractItemModel* servers READ servers NOTIFY serversChanged)
    Q_PROPERTY(QAbstractItemModel* excludes READ excludes NOTIFY excludesChanged)
    Q_PROPERTY(MethodType method READ method WRITE setMethod NOTIFY methodChanged)
public:
    explicit QNetworkSettingsProxy(QObject *parent = Q_NULLPTR);

    enum MethodType {Direct, Auto, Manual};

    QUrl url() const;
    void setUrl(const QUrl& url);
    QAbstractItemModel* servers();
    void setServers(const QStringList& servers);
    QStringList servers() const;
    QAbstractItemModel* excludes();
    QStringList excludes() const;
    void setExcludes(const QStringList& excludes);
    MethodType method() const;
    void setMethod(const MethodType& method);

Q_SIGNALS:
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

class Q_DECL_EXPORT QNetworkSettingsWireless : public QObject
{
    Q_OBJECT
    Q_ENUMS(SecurityFlags)
    Q_PROPERTY(int signalStrength READ signalStrength WRITE setSignalStrength NOTIFY signalStrengthChanged)
    Q_PROPERTY(bool hidden READ hidden NOTIFY hiddenChanged)
    Q_PROPERTY(bool isOutOfRange READ outOfRange WRITE setOutOfRange NOTIFY outOfRangeChanged)
public:
    explicit QNetworkSettingsWireless(QObject* parent = Q_NULLPTR);

    enum SecurityFlags {None=1, WEP=2, WPA=4, WPA2=8};

    Q_INVOKABLE bool supportsSecurity(SecurityFlags security);
    bool hidden() const;
    void setHidden(const bool hidden);
    int signalStrength() const;
    void setSignalStrength(const int signalStrength);
    void setSecurity(const SecurityFlags security);
    void setOutOfRange(const bool aOutOfRange);
    bool outOfRange() const;

Q_SIGNALS:
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
