/****************************************************************************
**
** Copyright (C) 2015 Digia Plc
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
#include "qwifiutils_p.h"

QT_BEGIN_NAMESPACE

int QWifiUtils::hex2num(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

int QWifiUtils::hex2byte(const char *hex)
{
    int a, b;
    a = hex2num(*hex++);
    if (a < 0)
        return -1;
    b = hex2num(*hex++);
    if (b < 0)
        return -1;
    return (a << 4) | b;
}

// the logic of this function is taken from wpa_supplicant source code (BSD
// licensed code) see http://w1.fi/cgit file hostapd/src/utils/common.c
// For Ascii encoded string, any octet < 32 or > 127 is encoded as a "\x"
// followed by the hex representation of the octet. Exception chars are ",
// \, \e, \n, \r, \t which are escaped by a backslash
QString QWifiUtils::decodeHexEncoded(const QString &encoded)
{
    int maxlen = encoded.size() + 1;
    QByteArray buf;
    buf.resize(maxlen);
    const char *pos = encoded.toLocal8Bit().constData();
    int len = 0;
    int val;

    while (*pos) {
        if (len + 1 >= maxlen)
            break;
        switch (*pos) {
        case '\\':
            pos++;
            switch (*pos) {
            case '\\':
                buf[len++] = '\\';
                pos++;
                break;
            case '"':
                buf[len++] = '"';
                pos++;
                break;
            case 'n':
                buf[len++] = '\n';
                pos++;
                break;
            case 'r':
                buf[len++] = '\r';
                pos++;
                break;
            case 't':
                buf[len++] = '\t';
                pos++;
                break;
            case 'e':
                buf[len++] = '\033';
                pos++;
                break;
            case 'x':
                pos++;
                val = hex2byte(pos);
                if (val < 0) {
                    val = hex2num(*pos);
                    if (val < 0)
                        break;
                    buf[len++] = val;
                    pos++;
                } else {
                    buf[len++] = val;
                    pos += 2;
                }
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                val = *pos++ - '0';
                if (*pos >= '0' && *pos <= '7')
                    val = val * 8 + (*pos++ - '0');
                if (*pos >= '0' && *pos <= '7')
                    val = val * 8 + (*pos++ - '0');
                buf[len++] = val;
                break;
            default:
                break;
            }
            break;
        default:
            buf[len++] = *pos++;
            break;
        }
    }
    if (maxlen > len)
        buf[len] = '\0';

    return QString::fromUtf8(buf);
}

QT_END_NAMESPACE
