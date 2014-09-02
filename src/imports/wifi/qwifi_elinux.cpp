/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://qt.digia.com/
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://qt.digia.com/
**
****************************************************************************/
#include "qwifi_elinux.h"

#include <QtCore/QDebug>

#include "wpa-supplicant/wpa_ctrl.h"

#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>

static const char SUPP_CONFIG_FILE[]    = "/etc/wpa_supplicant.conf";
static const char IFACE_DIR[]           = "/var/run/wpa_supplicant/";
static const char WIFI[]                = "wlan0";

static struct wpa_ctrl *ctrl_conn;
static struct wpa_ctrl *monitor_conn;
// socket pair used to exit from a blocking read
static int exit_sockets[2];

static const char IFNAME[] = "IFNAME=";
#define IFNAMELEN (sizeof(IFNAME) - 1)
static const char WPA_EVENT_IGNORE[] = "CTRL-EVENT-IGNORE ";

int wifi_connect_on_socket_path(const char *path);
int wifi_ctrl_recv(char *reply, size_t *reply_len);
int wifi_wait_on_socket(char *buf, size_t buflen);
int wifi_send_command(const char *cmd, char *reply, size_t *reply_len);
void wifi_close_sockets();

int q_wifi_start_supplicant()
{
    // NOTE: supplicant started when bringing up the wifi interface in
    // QWifiManager::connectToBackend() by:
    // QProcess::execute(QStringLiteral("ifup")

    /* Clear out any stale socket files that might be left over. */
    //wpa_ctrl_cleanup();
    /* Reset sockets used for exiting from hung state */
    exit_sockets[0] = exit_sockets[1] = -1;
    return 0;
}

int q_wifi_stop_supplicant()
{
    // NOTE: supplicant stopped when bringing down the wifi
    // interface in QWifiManager::disconnectFromBackend() by:
    // QProcess::execute(QStringLiteral("ifdown")
    return 0;
}

int q_wifi_connect_to_supplicant(const char *ifname)
{
    Q_UNUSED(ifname);
    static char path[4096];
    snprintf(path, sizeof(path), "%s/%s", IFACE_DIR, WIFI);
    return wifi_connect_on_socket_path(path);
}

int wifi_connect_on_socket_path(const char *path)
{
    // establishes the control and monitor socket connections on the interface
    ctrl_conn = wpa_ctrl_open(path);
    if (ctrl_conn == NULL) {
        qWarning("Unable to open connection to supplicant on \"%s\": %s",
             path, strerror(errno));
        return -1;
    }
    monitor_conn = wpa_ctrl_open(path);
    if (monitor_conn == NULL) {
        wpa_ctrl_close(ctrl_conn);
        ctrl_conn = NULL;
        return -1;
    }
    if (wpa_ctrl_attach(monitor_conn) != 0) {
        wpa_ctrl_close(monitor_conn);
        wpa_ctrl_close(ctrl_conn);
        ctrl_conn = monitor_conn = NULL;
        return -1;
    }

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, exit_sockets) == -1) {
        wpa_ctrl_close(monitor_conn);
        wpa_ctrl_close(ctrl_conn);
        ctrl_conn = monitor_conn = NULL;
        return -1;
    }

    return 0;
}

int q_wifi_wait_for_event(const char *ifname, char *buf, size_t buflen)
{
    Q_UNUSED(ifname);
    return wifi_wait_on_socket(buf, buflen);
}

int wifi_wait_on_socket(char *buf, size_t buflen)
{
    size_t nread = buflen - 1;
    int result;
    char *match, *match2;

    if (monitor_conn == NULL) {
        return snprintf(buf, buflen, WPA_EVENT_TERMINATING " - connection closed");
    }

    result = wifi_ctrl_recv(buf, &nread);

    /* Terminate reception on exit socket */
    if (result == -2) {
        return snprintf(buf, buflen, WPA_EVENT_TERMINATING " - connection closed");
    }

    if (result < 0) {
        qWarning("wifi_ctrl_recv failed: %s\n", strerror(errno));
        return snprintf(buf, buflen, WPA_EVENT_TERMINATING " - recv error");
    }
    buf[nread] = '\0';
    /* Check for EOF on the socket */
    if (result == 0 && nread == 0) {
        /* Fabricate an event to pass up */
        qWarning("Received EOF on supplicant socket\n");
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

    if (strncmp(buf, IFNAME, IFNAMELEN) == 0) {
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
            return snprintf(buf, buflen, "%s", WPA_EVENT_IGNORE);
        }
    } else if (buf[0] == '<') {
        match = strchr(buf, '>');
        if (match != NULL) {
            nread -= (match + 1 - buf);
            memmove(buf, match + 1, nread + 1);
            //qWarning("supplicant generated event without interface - %s\n", buf);
        }
    } else {
        /* let the event go as is! */
        qWarning("supplicant generated event without interface and without message level - %s\n", buf);
    }

    return nread;
}

int wifi_ctrl_recv(char *reply, size_t *reply_len)
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
        qWarning("Error poll = %d", res);
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

int wifi_send_command(const char *cmd, char *reply, size_t *reply_len)
{
    int ret;
    if (ctrl_conn == NULL) {
        qWarning("Not connected to wpa_supplicant - \"%s\" command dropped.\n", cmd);
        return -1;
    }
    ret = wpa_ctrl_request(ctrl_conn, cmd, strlen(cmd), reply, reply_len, NULL);
    if (ret == -2) {
        qWarning("'%s' command timed out.\n", cmd);
        /* unblocks the monitor receive socket for termination */
        TEMP_FAILURE_RETRY(write(exit_sockets[0], "T", 1));
        return -2;
    } else if (ret < 0 || strncmp(reply, "FAIL", 4) == 0) {
        return -1;
    }
    if (strncmp(cmd, "PING", 4) == 0) {
        reply[*reply_len] = '\0';
    }
    return 0;
}

int q_wifi_command(const char *ifname, const char *command, char *reply, size_t *reply_len)
{
    Q_UNUSED(ifname);
    return wifi_send_command(command, reply, reply_len);
}

void q_wifi_close_supplicant_connection(const char *ifname)
{
    Q_UNUSED(ifname)
    wifi_close_sockets();
}

void wifi_close_sockets()
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
