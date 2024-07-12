// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#include "qnetworksettingsinterface_p.h"
#include "connman_technology_interface.h"
#include "connmancommon.h"

QT_BEGIN_NAMESPACE

QNetworkSettingsInterfacePrivate::QNetworkSettingsInterfacePrivate(QNetworkSettingsInterface* parent)
    : QObject(parent)
    , m_technology(nullptr)
    ,q_ptr(parent)
{
}

void QNetworkSettingsInterfacePrivate::initialize(const QString& path, const QVariantMap& properties)
{
    m_technology = new NetConnmanTechnologyInterface(QStringLiteral("net.connman"), path,
                                                  QDBusConnection::systemBus(), this);
    connect(m_technology, SIGNAL(PropertyChanged(QString,QDBusVariant)),
           this, SLOT(updateProperty(QString,QDBusVariant)));

    updateProperty(PropertyName, properties[PropertyName]);
    updateProperty(PropertyType, properties[PropertyType]);
    updateProperty(PropertyConnected, properties[PropertyConnected]);
    updateProperty(PropertyPowered, properties[PropertyPowered]);
}

void QNetworkSettingsInterfacePrivate::updateProperty(const QString &name, const QDBusVariant &value)
{
    updateProperty(name, value.variant());
}

void QNetworkSettingsInterfacePrivate::updateProperty(const QString &name, const QVariant &value)
{
    Q_Q(QNetworkSettingsInterface);
    if (name == PropertyName) {
        m_name = qdbus_cast<QString>(value);
    }
    else if (name == PropertyType) {
        qdbus_cast<QString>(value) >> m_type;
        emit q->typeChanged();
    }
    else if (name == PropertyConnected) {
        bool connected = qdbus_cast<bool>(value);
        if (connected)
            m_state.setState(QNetworkSettingsState::Online);
        else
            m_state.setState(QNetworkSettingsState::Disconnect);
        emit q->stateChanged();
    }
    else if (name == PropertyPowered) {
        m_powered = qdbus_cast<bool>(value);
        emit q->poweredChanged();
    }
}

void QNetworkSettingsInterfacePrivate::setState(QNetworkSettingsState::State aState)
{
    Q_Q(QNetworkSettingsInterface);
    m_state.setState(aState);
    emit q->stateChanged();
}

void QNetworkSettingsInterfacePrivate::setPowered(const bool aPowered)
{
    m_technology->SetProperty(PropertyPowered, QDBusVariant(QVariant(aPowered)));
}

void QNetworkSettingsInterfacePrivate::scan()
{
    if (m_powered)
        m_technology->Scan();
    else
        qDebug() << "Tried to scan while not powered";
}

QString QNetworkSettingsInterfacePrivate::path() const
{
    return m_technology->path();
}

QT_END_NAMESPACE
