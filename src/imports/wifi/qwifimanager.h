#ifndef QWIFIMANAGER_H
#define QWIFIMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>

#include "qwifinetworklist.h"

class QWifiManagerEventThread;

class QWifiManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool ready READ isReady NOTIFY readyChanged)
    Q_PROPERTY(bool online READ isOnline NOTIFY onlineChanged)
    Q_PROPERTY(bool scanning READ scanning WRITE setScanning NOTIFY scanningChanged)

    Q_PROPERTY(QString connectedSSID READ connectedSSID NOTIFY connectedSSIDChanged)
    Q_PROPERTY(QWifiNetworkList *networks READ networks CONSTANT)

public:
    enum InternalState {
        IS_Uninitialized,
        IS_LoadDriver,
        IS_StartBackend,
        IS_ConnectToBackend,
        IS_UpAndRunning
    };

    QWifiManager();

    QWifiNetworkList *networks() { return &m_networks; }

    QString connectedSSID() const { return m_connectedSSID; }

    bool scanning() const { return m_scanning; }
    void setScanning(bool scanning);

    bool isReady() const { return m_internalState == IS_UpAndRunning; }
    bool isOnline() const { return m_online; }


public slots:
    void start();

    void connect(QWifiNetwork *network, const QString &passphrase);

signals:
    void scanningChanged(bool arg);
    void readyChanged(bool ready);
    void onlineChanged(bool online);
    void connectedSSIDChanged(const QString &);

protected:
    bool event(QEvent *);

private:
    friend class QWifiManagerEventThread;

    void handleConnected();
    void parseScanResults();
    void connectToBackend();
    QByteArray call(const char *command);
    bool checkedCall(const char *command);

    QString m_connectedSSID;
    QWifiNetworkList m_networks;

    QWifiManagerEventThread *m_eventThread;

    int m_scanTimer;

    InternalState m_internalState;

    bool m_scanning;
    bool m_online;

};

#endif // QWIFIMANAGER_H
