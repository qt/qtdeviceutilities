// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QNETWORKSETTINGS_H
#define QNETWORKSETTINGS_H

#include <QtNetworkSettings/qnetworksettingsglobal.h>

#include <QtCore/qobject.h>
#include <QtCore/qurl.h>
#include <QtNetworkSettings/qnetworksettingsaddressmodel.h>

QT_BEGIN_NAMESPACE

class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)

public:
    enum State {
        Idle = 0,
        Failure,
        Association,
        Configuration,
        Ready,
        Disconnect,
        Online,
        Undefined
    };
    Q_ENUM(State)

    explicit QNetworkSettingsState(State state, QObject *parent = nullptr);
    explicit QNetworkSettingsState(QObject *parent = nullptr);

    State state() const;
    void setState(const State state);

Q_SIGNALS:
    void stateChanged();

private:
    State m_state;
};

class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsType : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged)

public:
    enum Type {
        Wired = 0,
        Wifi,
        Bluetooth,
        Unknown
    };
    Q_ENUM(Type)

    explicit QNetworkSettingsType(Type type, QObject *parent = nullptr);
    explicit QNetworkSettingsType(QObject *parent = nullptr);

    Type type() const;
    void setType(const Type type);

Q_SIGNALS:
    void typeChanged();

private:
    Type m_type;
};

class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsIPv4 : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString gateway READ gateway WRITE setGateway NOTIFY gatewayChanged)
    Q_PROPERTY(Method method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(QString mask READ mask WRITE setMask NOTIFY maskChanged)

public:
    explicit QNetworkSettingsIPv4(QObject *parent = nullptr);

    enum Method {
        Dhcp = 0,
        Manual,
        Off
    };
    Q_ENUM(Method)

    QString address() const;
    void setAddress(const QString& address);
    QString gateway() const;
    void setGateway(const QString& gateway);
    Method method() const;
    void setMethod(const Method method);
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
    Method m_method;
    QString m_mask;
};

class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsIPv6 : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString gateway READ gateway WRITE setGateway NOTIFY gatewayChanged)
    Q_PROPERTY(Method method READ method WRITE setMethod NOTIFY methodChanged)
    Q_PROPERTY(Privacy privacy READ privacy WRITE setPrivacy NOTIFY privacyChanged)
    Q_PROPERTY(int prefixLength READ prefixLength WRITE setPrefixLength NOTIFY prefixLengthChanged)

public:
    explicit QNetworkSettingsIPv6(QObject *parent = nullptr);

    enum Method {
        Auto = 0,
        Manual,
        Off
    };
    Q_ENUM(Method)

    enum Privacy {
        Disabled = 0,
        Enabled,
        Preferred
    };
    Q_ENUM(Privacy)

    QString address() const;
    void setAddress(const QString& address);
    QString gateway() const;
    void setGateway(const QString& gateway);
    Method method() const;
    void setMethod(const Method method);
    Privacy privacy() const;
    void setPrivacy(const Privacy privacy);
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
    Method m_method;
    Privacy m_privacy;
    int m_prefixLength;
};

class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsProxy : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QAbstractItemModel* servers READ servers NOTIFY serversChanged)
    Q_PROPERTY(QAbstractItemModel* excludes READ excludes NOTIFY excludesChanged)
    Q_PROPERTY(Method method READ method WRITE setMethod NOTIFY methodChanged)
public:
    explicit QNetworkSettingsProxy(QObject *parent = nullptr);

    enum Method {
        Direct = 0,
        Auto,
        Manual
    };
    Q_ENUM(Method)

    QUrl url() const;
    void setUrl(const QUrl& url);
    QAbstractItemModel* servers();
    void setServers(const QStringList& servers);
    QStringList servers() const;
    QAbstractItemModel* excludes();
    QStringList excludes() const;
    void setExcludes(const QStringList& excludes);
    Method method() const;
    void setMethod(const Method& method);

Q_SIGNALS:
    void urlChanged();
    void serversChanged();
    void excludesChanged();
    void methodChanged();
private:
    QUrl m_url;
    QNetworkSettingsAddressModel m_servers;
    QNetworkSettingsAddressModel m_excludes;
    Method m_method;
};

class Q_NETWORKSETTINGS_EXPORT QNetworkSettingsWireless : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int signalStrength READ signalStrength WRITE setSignalStrength NOTIFY signalStrengthChanged)
    Q_PROPERTY(bool hidden READ hidden NOTIFY hiddenChanged)
    Q_PROPERTY(bool isOutOfRange READ outOfRange WRITE setOutOfRange NOTIFY outOfRangeChanged)
public:
    explicit QNetworkSettingsWireless(QObject* parent = nullptr);

    enum class Security {
        None    = 0x0000,
        WEP     = 0x0002,
        WPA     = 0x0004,
        WPA2    = 0x0008
    };
    Q_DECLARE_FLAGS(Securities, Security)

    Q_INVOKABLE bool supportsSecurity(Security security);
    bool hidden() const;
    void setHidden(const bool hidden);
    int signalStrength() const;
    void setSignalStrength(const int signalStrength);
    void setSecurity(const Securities securities);
    void setOutOfRange(const bool aOutOfRange);
    bool outOfRange() const;

Q_SIGNALS:
    void hiddenChanged();
    void signalStrengthChanged();
    void passwordChanged();
    void outOfRangeChanged();
private:
    Securities m_supportedSecurites;
    bool m_hidden;
    int m_signalStrength;
    bool m_isOutOfRange;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(QNetworkSettingsWireless::Securities)

QT_END_NAMESPACE

#endif //QNETWORKSETTINGS_H
