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
#include "qwifisupplicant_p.h"
#include "qwifidevice.h"

#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>

#include <QtCore/QFile>
#include <QtCore/QProcess>

QT_BEGIN_NAMESPACE

Q_LOGGING_CATEGORY(B2QT_WIFI_VERBOSE, "qt.b2qt.wifi.verbose")

#define CONFIG_FILE "/etc/wpa_supplicant.qtwifi.conf"
#define CONTROL_INTERFACE_PATH "/var/run/wpa_supplicant/"

QWifiSupplicant::QWifiSupplicant(QObject *parent) :
    QObject(parent),
    ctrl_conn(0),
    monitor_conn(0),
    interface(QWifiDevice::wifiInterfaceName())
{
    createSupplicantConfig();
}

void QWifiSupplicant::createSupplicantConfig() const
{
    QFile supplicantConfig(QLatin1String(CONFIG_FILE));
    if (supplicantConfig.exists())
        return;

    if (supplicantConfig.open(QIODevice::WriteOnly)) {
        supplicantConfig.write("ctrl_interface=" CONTROL_INTERFACE_PATH "\n"
                               "ctrl_interface_group=0\n"
                               "update_config=1\n");
    } else {
        qCWarning(B2QT_WIFI) << "failed to create supplicant configuration file.";
    }
}

bool QWifiSupplicant::startSupplicant()
{
    QString pidFile = QLatin1String("/var/run/wpa_supplicant." + interface + ".pid");
    QString driver(QStringLiteral("nl80211,wext"));

    QStringList arg;
    arg << QStringLiteral("--start") << QStringLiteral("--quiet") << QStringLiteral("--name");
    arg << QStringLiteral("wpa_supplicant") << QStringLiteral("--startas");
    arg << QStringLiteral("/usr/sbin/wpa_supplicant") << QStringLiteral("--pidfile") << pidFile;
    arg << QStringLiteral("--") << QStringLiteral("-B") << QStringLiteral("-P") << pidFile;
    arg << QStringLiteral("-i") << QLatin1String(interface) << QStringLiteral("-c");
    arg << QLatin1String(CONFIG_FILE) << QStringLiteral("-D") << driver;

    QProcess startStopDaemon;
    startStopDaemon.start(QStringLiteral("start-stop-daemon"), arg);
    startStopDaemon.waitForFinished();
    qCDebug(B2QT_WIFI) << startStopDaemon.readAll();

    // if the interface socket exists then wpa-supplicant was invoked successfully
    if (!QFile(QLatin1String(CONTROL_INTERFACE_PATH + interface)).exists()) {
        qCWarning(B2QT_WIFI) << "failed to invoke wpa_supplicant!\n" << startStopDaemon.readAll();
        return false;
    }
    // reset sockets used for exiting from hung state
    exit_sockets[0] = exit_sockets[1] = -1;
    return true;
}

bool QWifiSupplicant::stopSupplicant()
{
    QString pidFile = QLatin1String("/var/run/wpa_supplicant." + interface + ".pid");

    if (QFile(pidFile).exists()) {
        QStringList arg;
        arg << QStringLiteral("--stop") << QStringLiteral("--quiet") << QStringLiteral("--name");
        arg << QStringLiteral("wpa_supplicant") << QStringLiteral("--pidfile") << pidFile;

        QProcess startStopDaemon;
        startStopDaemon.start(QStringLiteral("start-stop-daemon"), arg);
        startStopDaemon.waitForFinished();
        if (startStopDaemon.exitStatus() != QProcess::NormalExit) {
            qCWarning(B2QT_WIFI) << "failed to stop a supplicant process!\n" << startStopDaemon.readAll();
            return false;
        }

        QFile::remove(pidFile);
    }

    QFile::remove(QLatin1String(CONTROL_INTERFACE_PATH + interface));

    // workaround for QTEE-957
    QProcess killall;
    killall.start(QStringLiteral("killall"), QStringList() << QStringLiteral("-9") << QStringLiteral("wpa_supplicant"));
    killall.waitForFinished();

    return true;
}

bool QWifiSupplicant::connectToSupplicant()
{
    static char path[4096];
    snprintf(path, sizeof(path), "%s/%s", CONTROL_INTERFACE_PATH, interface.constData());

    ctrl_conn = wpa_ctrl_open(path);
    if (ctrl_conn == NULL) {
        qCWarning(B2QT_WIFI, "Unable to open connection to supplicant on \"%s\": %s",
                  path, strerror(errno));
        return false;
    }
    monitor_conn = wpa_ctrl_open(path);
    if (monitor_conn == NULL) {
        wpa_ctrl_close(ctrl_conn);
        ctrl_conn = NULL;
        return false;
    }
    if (wpa_ctrl_attach(monitor_conn) != 0) {
        wpa_ctrl_close(monitor_conn);
        wpa_ctrl_close(ctrl_conn);
        ctrl_conn = monitor_conn = NULL;
        return false;
    }

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, exit_sockets) == -1) {
        wpa_ctrl_close(monitor_conn);
        wpa_ctrl_close(ctrl_conn);
        ctrl_conn = monitor_conn = NULL;
        return false;
    }

    return true;
}

void QWifiSupplicant::closeSupplicantConnection()
{
    if (ctrl_conn != NULL) {
        wpa_ctrl_close(ctrl_conn);
        ctrl_conn = NULL;
    }

    if (monitor_conn != NULL) {
        wpa_ctrl_close(monitor_conn);
        monitor_conn = NULL;
    }

    if (exit_sockets[0] >= 0) {
        close(exit_sockets[0]);
        exit_sockets[0] = -1;
    }

    if (exit_sockets[1] >= 0) {
        close(exit_sockets[1]);
        exit_sockets[1] = -1;
    }
}

int QWifiSupplicant::waitForEvent(char *buf, size_t buflen)
{
    size_t nread = buflen - 1;
    int result;
    char *match, *match2;

    if (monitor_conn == NULL)
        return snprintf(buf, buflen, WPA_EVENT_TERMINATING " - connection closed");

    result = receiveEvent(buf, &nread);

    // Terminate reception on exit socket
    if (result == -2)
        return snprintf(buf, buflen, WPA_EVENT_TERMINATING " - connection closed");

    if (result < 0) {
        qCWarning(B2QT_WIFI, "receiveEvent failed: %s", strerror(errno));
        return snprintf(buf, buflen, WPA_EVENT_TERMINATING " - recv error");
    }

    buf[nread] = '\0';
    // Check for EOF on the socket
    if (result == 0 && nread == 0) {
        // Fabricate an event to pass up
        qCWarning(B2QT_WIFI, "Received EOF on supplicant socket");
        return snprintf(buf, buflen, WPA_EVENT_TERMINATING " - signal 0 received");
    }

    /*
     * Events strings are in the format
     *
     *     IFNAME=iface <N>CTRL-EVENT-XXX
     *        or
     *     <N>CTRL-EVENT-XXX
     *
     * where N is the message level in numerical form (0=VERBOSE, 1=DEBUG,
     * etc.) and XXX is the event name. The level information is not useful
     * to us, so strip it off.
     */

    if (strncmp(buf, "IFNAME=", (sizeof("IFNAME=") - 1)) == 0) {
        match = strchr(buf, ' ');
        if (match != NULL) {
            if (match[1] == '<') {
                match2 = strchr(match + 2, '>');
                if (match2 != NULL) {
                    nread -= (match2 - match);
                    memmove(match + 1, match2 + 1, nread - (match - buf) + 1);
                }
            }
        } else {
            return snprintf(buf, buflen, "%s", "CTRL-EVENT-IGNORE ");
        }
    } else if (buf[0] == '<') {
        match = strchr(buf, '>');
        if (match != NULL) {
            nread -= (match + 1 - buf);
            memmove(buf, match + 1, nread + 1);
            //qCWarning(B2QT_WIFI, "supplicant generated event without interface - %s", buf);
        }
    } else {
        // let the event go as is!
        qCWarning(B2QT_WIFI, "supplicant generated event without interface and without message level - %s", buf);
    }

    return nread;
}

bool QWifiSupplicant::sendCommand(const QString &command, QByteArray *reply)
{
    QByteArray cmd = command.toLocal8Bit();
    qCDebug(B2QT_WIFI) << "[command]: " << cmd;

    if (ctrl_conn == NULL) {
        qCWarning(B2QT_WIFI, "Not connected to wpa_supplicant");
        return false;
    }

    char data[8192];
    size_t len = sizeof(data) - 1; // -1: room to add a 0-terminator
    int ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), data, &len, NULL);
    if (ret == -2) {
        qCWarning(B2QT_WIFI) << "command timed out";
        // unblocks the monitor receive socket for termination
        TEMP_FAILURE_RETRY(write(exit_sockets[0], "T", 1));
        return false;
    } else if (ret < 0 || strncmp(data, "FAIL", 4) == 0) {
        return false;
    }

    if (len == sizeof(data) - 1) {
        qCWarning(B2QT_WIFI) << "possible buffer overflow detected!";
        return false;
    }

    data[len] = 0;
    qCDebug(B2QT_WIFI_VERBOSE) << "[response]: " << data;
    *reply = QByteArray(data, len);

    return true;
}

int QWifiSupplicant::receiveEvent(char *reply, size_t *reply_len)
{
    int res = 0;
    int ctrlfd = wpa_ctrl_get_fd(monitor_conn);
    struct pollfd rfds[2];

    memset(rfds, 0, 2 * sizeof(struct pollfd));
    rfds[0].fd = ctrlfd;
    rfds[0].events |= POLLIN;
    rfds[1].fd = exit_sockets[1];
    rfds[1].events |= POLLIN;
    res = TEMP_FAILURE_RETRY(poll(rfds, 2, -1));
    if (res < 0) {
        qCWarning(B2QT_WIFI, "Error poll = %d", res);
        return res;
    }
    if (rfds[0].revents & POLLIN) {
        return wpa_ctrl_recv(monitor_conn, reply, reply_len);
    }

    /* it is not rfds[0], then it must be rfts[1] (i.e. the exit socket)
     * or we timed out. In either case, this call has failed ..
     */
    return -2;
}

/*! \internal
 *
    Decode wpa_supplicant encoded string, see file hostapd/src/utils/common.c
    in git://w1.fi/hostap.git repository.

    For Ascii encoded string, any octet < 32 or > 127 is encoded as a "\\x"
    followed by the hex representation of the octet. Exception chars are ",
    \\, \\e, \\n, \\r, \\t which are escaped by a backslash

 */
QString QWifiSupplicant::decodeHexEncoded(const QString &encoded)
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

int QWifiSupplicant::hex2num(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

int QWifiSupplicant::hex2byte(const char *hex)
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

QT_END_NAMESPACE
