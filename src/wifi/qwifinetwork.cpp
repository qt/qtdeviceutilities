/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://www.qt.io
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://www.qt.io
**
****************************************************************************/
#include "qwifinetwork_p.h"

QT_BEGIN_NAMESPACE

QWifiNetwork::QWifiNetwork(QObject *parent)
    : QObject(parent)
    , m_isOutOfRange(false)
{
}

QWifiNetwork::~QWifiNetwork()
{
}

void QWifiNetwork::setSsid(const QString &ssid)
{
    m_ssid = ssid;
}

void QWifiNetwork::setSignalStrength(int strength)
{
    m_signalStrength = strength;
}

void QWifiNetwork::setOutOfRange(bool outOfRange)
{
    m_isOutOfRange = outOfRange;
}

QT_END_NAMESPACE
