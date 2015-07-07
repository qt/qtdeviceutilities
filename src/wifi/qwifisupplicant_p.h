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
#ifndef QWIFISUPPLICANT_H
#define QWIFISUPPLICANT_H

#include <QObject>
#include <QByteArray>

#include "wpa-supplicant/wpa_ctrl.h"

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(B2QT_WIFI)
Q_DECLARE_LOGGING_CATEGORY(B2QT_WIFI_VERBOSE)

class QWifiSupplicant : public QObject
{
    Q_OBJECT
public:
    explicit QWifiSupplicant(QObject *parent = 0);

    void createSupplicantConfig() const;
    bool startSupplicant();
    bool stopSupplicant();
    bool connectToSupplicant();
    void closeSupplicantConnection();
    int waitForEvent(char *buf, size_t buflen);
    bool sendCommand(const QString &command, QByteArray *reply);
    static QString decodeHexEncoded(const QString &encoded);

protected:
    int receiveEvent(char *reply, size_t *reply_len);
    static int hex2num(char c);
    static int hex2byte(const char *hex);

private:
    wpa_ctrl *ctrl_conn;
    wpa_ctrl *monitor_conn;
    int exit_sockets[2];
    QByteArray interface;
};

QT_END_NAMESPACE

#endif // QWIFISUPPLICANT_H
