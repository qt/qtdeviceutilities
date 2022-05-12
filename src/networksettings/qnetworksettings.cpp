/****************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Device Utilities module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
******************************************************************************/
#include "qnetworksettings.h"

QT_BEGIN_NAMESPACE

/*!
    \module QtNetworkSettings
    \qtvariable networksettings
    \ingroup qtdevice-utilities-cpp-modules
    \ingroup modules
    \title Qt Network Settings C++ Classes
    \brief Provides functionality for controlling network settings.

    To use classes from this module, add this directive into the C++ files:

    \code
    #include <QtNetworkSettings>
    \endcode

    To link against the corresponding C++ libraries when using CMake to build you application,
    add the following to your CMakeLists.txt file:

    \code
    find_package(Qt6 COMPONENTS NetworkSettings REQUIRED)
    target_link_libraries(mytarget Qt6::NetworkSettings)
    \endcode

    Or if you are using qmake to build your application, add the following line to your qmake .pro project file:

    \code
    QT += networksettings
    \endcode
*/

/*!
    \class QNetworkSettingsState
    \inmodule QtNetworkSettings

    \brief The QNetworkSettingsState class represents the network interface
    state.

    \sa QNetworkSettingsManager
*/

/*!
    \enum QNetworkSettingsState::State

    This enum type holds the state of the network interface.

    \value  Idle
    \value  Failure
            Failed to connect.
    \value  Association
            Authentication in progress.
    \value  Configuration
            Configuration in progress.
    \value  Ready
            Connected to a network.
    \value  Disconnect
            Disconnected from a network.
    \value  Online
            Acquired an IP address.
    \value  Undefined
            Undefined state.
*/

/*!
    \property QNetworkSettingsState::state
    \brief The state of the network interface.

    \sa QNetworkSettingsState::State
*/

/*!
    \fn QNetworkSettingsState::stateChanged()
    This signal is emitted when the state of the network interface changes.

    \sa QNetworkSettingsState::State
*/

/*!
    Creates a new network interface state object with the state \a state and the
    parent \a parent.
*/
QNetworkSettingsState::QNetworkSettingsState(State state, QObject *parent)
    : QObject(parent)
    , m_state(state)
{
}

/*!
    Creates a new network interface state object with the parent \a parent.
*/
QNetworkSettingsState::QNetworkSettingsState(QObject *parent)
    : QObject(parent)
    , m_state(Undefined)
{
}

/*!
    Returns the network interface state.
*/
QNetworkSettingsState::State QNetworkSettingsState::state() const {
    return m_state;
}

/*!
    Sets the network interface state to \a state.
*/
void QNetworkSettingsState::setState(const State state) {
    m_state = state;
    emit stateChanged();
}

/*!
    \class QNetworkSettingsType
    \inmodule QtNetworkSettings

    \brief The QNetworkSettingsType class represents the network interface
    type.

    \sa QNetworkSettingsManager
*/

/*!
    \enum QNetworkSettingsType::Type

    This enum type holds the type of the network interface.

    \value  Wired
            Wired network
    \value  Wifi
            Wifi network
    \value  Bluetooth
            Bluetooth network
    \value  Unknown
            Unknown network type
*/

/*!
    \property QNetworkSettingsType::type
    \brief The type of the network interface.

    \sa QNetworkSettingsType::Type
*/

/*!
    \fn QNetworkSettingsType::typeChanged()
    This signal is emitted when the type of the network interface changes.

    \sa QNetworkSettingsType::Type
*/

/*!
    Creates a new network interface type object with the type \a type and
    parent \a parent.
*/
QNetworkSettingsType::QNetworkSettingsType(Type type, QObject *parent)
    : QObject(parent)
    , m_type(type)
{
    m_type = type;
}

/*!
    Creates a new network interface type object with the parent \a parent.
*/
QNetworkSettingsType::QNetworkSettingsType(QObject *parent)
    : QObject(parent)
{
    m_type = Unknown;
}

/*!
    Returns the network interface type.
*/
QNetworkSettingsType::Type QNetworkSettingsType::type() const {
    return m_type;
}

/*!
    Sets the network interface type to \a type.
*/
void QNetworkSettingsType::setType(const Type type) {
    m_type = type;
    emit typeChanged();
}

/*!
    \class QNetworkSettingsIPv4
    \inmodule QtNetworkSettings

    \brief The QNetworkSettingsIPv4 class encapsulates IPv4 network
    configuration.
*/

/*!
    \enum QNetworkSettingsIPv4::Method

    This enum type holds the method used for IPv4 configuration.

    \value  Dhcp
            The DHCP protocol is used for the network configuration.
    \value  Manual
            The network is configured manually.
    \value  Off
            The network is not configured.
*/

/*!
    \property QNetworkSettingsIPv4::address
    \brief Holds the IPv4 address.
*/

/*!
    \property QNetworkSettingsIPv4::gateway
    \brief Holds the IPv4 gateway address.
*/

/*!
    \property QNetworkSettingsIPv4::method
    \brief Holds the method of IPv4 configuration.

    \sa QNetworkSettingsIPv4::Method
*/

/*!
    \property QNetworkSettingsIPv4::mask
    \brief Holds the IPv4 network mask.
*/

/*!
    Creates a new IPv4 network configuration with the parent \a parent.
*/
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

QNetworkSettingsIPv4::Method QNetworkSettingsIPv4::method() const {
    return m_method;
}

void QNetworkSettingsIPv4::setMethod(const Method method) {
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

/*!
    \class QNetworkSettingsIPv6
    \inmodule QtNetworkSettings

    \brief The QNetworkSettingsIPv6 class encapsulates IPv6 network
    configuration.
*/

/*!
    \enum QNetworkSettingsIPv6::Method

    This enum type holds the method used for IPv6 configuration.

    \value  Auto
            The network is configured automatically.
    \value  Manual
            The network is configured manually.
    \value  Off
            The network is not configured.
*/

/*!
    \enum QNetworkSettingsIPv6::Privacy
    \brief Holds the method of applying the privacy extensions in IPv6.

    \value  Disabled
            Disables privacy extensions in IPv6
    \value  Enabled
            Enables \l {https://tools.ietf.org/html/rfc4941}
            {Privacy Extensions for Stateless Address Autoconfiguration in IPv6}.
    \value  Preferred
            Enables privacy extensions and gives preference to the use of
            temporary addresses, even when a public address is available
*/

/*!
    \property QNetworkSettingsIPv6::address
    \brief Holds the IPv6 address.
*/

/*!
    \property QNetworkSettingsIPv6::gateway
    \brief Holds the IPv6 gateway address.
*/

/*!
    \property QNetworkSettingsIPv6::method
    \brief Holds the method of IPv6 configuration.

    \sa QNetworkSettingsIPv6::Method
*/

/*!
    \property QNetworkSettingsIPv6::privacy
    \brief Holds the method of applying privacy extensions for IPv6.

    \sa QNetworkSettingsIPv6::Privacy
*/

/*!
    \property QNetworkSettingsIPv6::prefixLength
    \brief Holds the IPv6 network prefix length in bits.
*/


/*!
    Creates a new IPv6 network configuration with the parent \a parent.
*/
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

QNetworkSettingsIPv6::Method QNetworkSettingsIPv6::method() const {
    return m_method;
}

void QNetworkSettingsIPv6::setMethod(const Method method) {
    m_method = method;
    emit methodChanged();
}

QNetworkSettingsIPv6::Privacy QNetworkSettingsIPv6::privacy() const {
    return m_privacy;
}

void QNetworkSettingsIPv6::setPrivacy(const Privacy privacy) {
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


/*!
    \class QNetworkSettingsProxy
    \inmodule QtNetworkSettings
    \brief The QNetworkSettingsProxy class encapsulates network proxy
    configuration.
*/

/*!
    \enum QNetworkSettingsProxy::Method
    \brief Holds the network proxy configuration method.

    \value  Direct
            Direct network connection, no proxy in use
    \value  Auto
            Automatic proxy configuration
    \value  Manual
            Manual proxy configuration

    \sa url
*/

/*!
    \property QNetworkSettingsProxy::method
    \brief Holds the network proxy configuration method.

    \sa QNetworkSettingsProxy::Method
*/

/*!
    \property QNetworkSettingsProxy::url
    \brief Holds the proxy URL.

    For manual proxy configuration, this property holds the proxy server
    address. For automatic configuration, it holds the proxy auto-config URL.

    \sa QNetworkSettingsProxy::Method
*/

/*!
    \property QNetworkSettingsProxy::excludes
    \readonly
    \brief The model containing the proxy exclusion list.

    The addresses in the proxy exclusion list are accessed directly,
    instead of forwarding the requests to a proxy.

    This property can be used as a model for a view
    that lists the proxy exclusion addresses.

    \sa QNetworkSettingsAddressModel
*/

/*!
    \property QNetworkSettingsProxy::servers
    \readonly
    \brief The model containing the proxy exclusion list.

    This property holds the list of proxy server names.
*/

/*!
    Creates a new proxy configuration with the parent \a parent.
*/
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

/*!
    Sets the proxy server list.
    \a servers lists the server names.

    The QNetworkSettingsProxy::serversChanged signal is emitted when the server list
    is set.
*/
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

/*!
    Sets the proxy exclusion list.
    \a excludes lists the addresses on the exclustion list.

    The QNetworkSettingsProxy::excludesChanged signal is emitted when the proxy
    exclusion list is set.
*/
void QNetworkSettingsProxy::setExcludes(const QStringList& excludes) {
    m_excludes.setStringList(excludes);
    emit excludesChanged();
}

QNetworkSettingsProxy::Method QNetworkSettingsProxy::method(void) const {
    return m_method;
}

void QNetworkSettingsProxy::setMethod(const Method& method) {
    m_method = method;
    emit methodChanged();
}

/*!
    \class QNetworkSettingsWireless
    \inmodule QtNetworkSettings
    \brief The QNetworkSettingsWireless class encapsulates the configuration
    for a Wifi network service.
*/

/*!
    \enum QNetworkSettingsWireless::Security
    \brief This enum type holds the wireless security protocol used to
    protect the connection.

    \value  None
            Not protected
    \value  WEP
            Wired Equivalent Privacy (WEP)
    \value  WPA
            Wi-Fi Protected Access (WPA)
    \value  WPA2
            Wi-Fi Protected Access, version 2 (WPA2)
*/

/*!
    \property QNetworkSettingsWireless::signalStrength
    \brief Holds the Wifi signal strength, in the range from 0 to 100.
*/

/*!
    \property QNetworkSettingsWireless::hidden
    \readonly
    \brief Holds whether the wireless SSID is hidden.
*/

/*!
    \property QNetworkSettingsWireless::isOutOfRange
    \brief Holds whether the Wifi access point is out of range.
*/

/*!
    \fn void QNetworkSettingsWireless::passwordChanged()

    This signal is sent when the password has changed.
*/

/*!
    Creates a new Wifi network configuration with the parent \a parent.
*/
QNetworkSettingsWireless::QNetworkSettingsWireless(QObject* parent)
  : QObject(parent) {
}

/*!
    Returns whether the Wifi network supports the wireless security
    protocol specified in \a security.

    \sa QNetworkSettingsWireless::Security
*/
bool QNetworkSettingsWireless::supportsSecurity(Security security) {
    return m_supportedSecurites.testFlag(security);
}

bool QNetworkSettingsWireless::hidden() const {
    return m_hidden;
}

/*!
    Sets \a hidden status.

    The QNetworkSettingsWireless::hiddenChanged signal is emitted when the
    hidden status is changed.
*/
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

/*!
    Sets \a security flags.
*/
void QNetworkSettingsWireless::setSecurity(const Securities securities) {
    m_supportedSecurites = securities;
}

void QNetworkSettingsWireless::setOutOfRange(const bool aOutOfRange) {
    m_isOutOfRange = aOutOfRange;
    emit outOfRangeChanged();
}

bool QNetworkSettingsWireless::outOfRange() const {
    return m_isOutOfRange;
}

QT_END_NAMESPACE
