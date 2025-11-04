// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
#ifndef QNETWORKSETTINGSINTERFACEPRIVATE_H
#define QNETWORKSETTINGSINTERFACEPRIVATE_H

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

#include <QObject>
#include <QtDBus/QDBusVariant>
#include "qnetworksettings.h"
#include "qnetworksettingsinterface.h"

// Automatically generated class in global namespace
class NetConnmanTechnologyInterface;

QT_BEGIN_NAMESPACE

class QNetworkSettingsInterfacePrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(QNetworkSettingsInterface)
public:
    explicit QNetworkSettingsInterfacePrivate(QNetworkSettingsInterface* parent);
    void initialize(const QString& path, const QVariantMap& properties);
    void setPowered(const bool power);
    void setState(QNetworkSettingsState::State aState);
    void scan();
    QString name() const {return m_name;}
    QNetworkSettingsType::Type type() const {return m_type.type();}
    QNetworkSettingsState::State state() const {return m_state.state();}
    bool powered() const {return m_powered;}
    QString path() const;

public slots:
    void updateProperty(const QString &name, const QDBusVariant &value);
protected:
    void updateProperty(const QString &name, const QVariant &value);

protected:
    NetConnmanTechnologyInterface *m_technology;
    QString m_name;
    QNetworkSettingsType m_type;
    QNetworkSettingsState m_state;
    bool m_powered;
    QNetworkSettingsInterface *q_ptr;
};


class ConnmanSettingsInterface : public QNetworkSettingsInterface
{
    Q_OBJECT
public:
    ConnmanSettingsInterface(const QString& path, const QVariantMap& properties, QObject *parent = nullptr)
        :QNetworkSettingsInterface(parent)
    {
        if (d_ptr)
            d_ptr->initialize(path, properties);
    }

    void setState(QNetworkSettingsState::State aState) {
        Q_D(QNetworkSettingsInterface);
        d->setState(aState);
    }

    virtual ~ConnmanSettingsInterface() {}

    QString path() const {
        if (d_ptr)
            return d_ptr->path();
        return QString();
    }
};

QT_END_NAMESPACE

#endif // QNETWORKSETTINGSINTERFACEPRIVATE_H
