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
#include <QtCore>
#include <QtWidgets>
#include <B2QtWifi>

class NetworkDelegate : public QStyledItemDelegate
{
    Q_OBJECT
//! [1]
public:
    NetworkDelegate(QObject *parent = 0)
        : QStyledItemDelegate(parent)
    {
        m_wifiManager = QWifiManager::instance();
    }

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QStyledItemDelegate::paint(painter, option, index);
        painter->save();
        QString ssid = qvariant_cast<QString>(index.data(QWifiManager::SSID));
        if (ssid == m_wifiManager->currentSSID())
            ssid += networkStateText();
        painter->drawText(option.rect, Qt::AlignVCenter, ssid);
        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QFont font = QApplication::font();
        QFontMetrics fm(font);
        return QSize(option.rect.width(), fm.height() * 2);
    }

protected:
    QString networkStateText() const
    {
        QWifiManager::NetworkState networkState = m_wifiManager->networkState();
        switch (networkState) {
        case QWifiManager::Disconnected:
            return QStringLiteral("");
            break;
        case QWifiManager::Authenticating:
            return QStringLiteral(" (authenticating)");
            break;
        case QWifiManager::HandshakeFailed:
            return QStringLiteral(" (handshake failed)");
            break;
        case QWifiManager::ObtainingIPAddress:
            return QStringLiteral(" (obtaining IP address)");
            break;
        case QWifiManager::DhcpRequestFailed:
            return QStringLiteral(" (dhcp request failed)");
            break;
        case QWifiManager::Connected:
            return QStringLiteral(" (connected)");
            break;
        }
    }
//! [1]
private:
    QWifiManager *m_wifiManager;
};

class WifiSettings : public QWidget
{
    Q_OBJECT
public:
    //! [0]
    WifiSettings(QWidget *parent = 0)
        : QWidget(parent)
        , m_listView(new QListView(this))
        , m_networkDelegate(new NetworkDelegate(this))
    {
        m_wifiManager = QWifiManager::instance();
        m_listView->setModel(m_wifiManager->networks());
        m_listView->setItemDelegate(m_networkDelegate);

        connect(m_wifiManager, &QWifiManager::backendStateChanged,
                this, &WifiSettings::handleBackendStateChanged);
        connect(m_wifiManager, &QWifiManager::networkStateChanged,
                this, &WifiSettings::handleNetworkStateChanged);

        setupWidgets();
    }
    //! [0]
protected:
    void setupWidgets()
    {
        m_backendStateReporter = new QLabel(this);
        handleBackendStateChanged(m_wifiManager->backendState());
        m_passwordInput = new QLineEdit(this);
        m_passwordInput->setPlaceholderText("Enter Password");

        QPushButton *connectButton = new QPushButton("Connect", this);
        QPushButton *disconnectButton = new QPushButton("Disconnect", this);
        QPushButton *startBackendButton = new QPushButton("Switch On", this);
        QPushButton *stopBackendButton = new QPushButton("Switch Off", this);

        connect(startBackendButton, &QPushButton::clicked, m_wifiManager, &QWifiManager::start);
        connect(stopBackendButton, &QPushButton::clicked, m_wifiManager, &QWifiManager::stop);
        connect(connectButton, &QPushButton::clicked, this, &WifiSettings::connectToNetwork);
        connect(disconnectButton, &QPushButton::clicked, m_wifiManager, &QWifiManager::disconnect);

        QGridLayout *grid = new QGridLayout(this);
        grid->addWidget(connectButton, 0, 0);
        grid->addWidget(disconnectButton, 0, 1);
        grid->addWidget(startBackendButton, 1, 0);
        grid->addWidget(stopBackendButton, 1, 1);
        grid->addWidget(m_listView, 2, 0, 1, 2);
        grid->addWidget(m_passwordInput, 3, 0, 1, 2);
        grid->addWidget(m_backendStateReporter, 4, 0, 1, 2);
        setLayout(grid);
    }


protected slots:
    void handleBackendStateChanged(QWifiManager::BackendState state)
    {
        switch (state) {
        case QWifiManager::Running:
            m_wifiManager->setScanning(true);
            m_backendStateReporter->setText("wifi backend state: <b>running<\b>");
            break;
        case QWifiManager::NotRunning:
             m_wifiManager->setScanning(false);
             m_backendStateReporter->setText("wifi backend state: <b>stopped<\b>");
            break;
        case QWifiManager::Initializing:
            m_backendStateReporter->setText("wifi backend state: <b>initializing<\b>");
            break;
        case QWifiManager::Terminating:
            m_backendStateReporter->setText("wifi backend state: <b>terminating<\b>");
            break;
        }
    }
    //! [4]
    void handleNetworkStateChanged(QWifiManager::NetworkState state)
    {
        m_listView->viewport()->repaint();
    }
    //! [4]
    //! [2]
    void connectToNetwork()
    {
        QModelIndex index = m_listView->currentIndex();
        QWifiConfiguration config;
        if (index.isValid()) {
            QString ssid = qvariant_cast<QString>(index.data(QWifiManager::SSID));
            config.setSsid(ssid);
            config.setPassphrase(m_passwordInput->text());
            m_wifiManager->connect(&config);
        }
    }
    //! [2]
private:
    QWifiManager *m_wifiManager;
    QListView *m_listView;
    NetworkDelegate *m_networkDelegate;
    QLabel *m_backendStateReporter;
    QLineEdit *m_passwordInput;
};

//! [3]
class WifiConnectionHandler : public QObject
{
    Q_OBJECT
public:
    WifiConnectionHandler()
    {
        // replace with a valid network configuration
        m_config.setSsid("my-local-wifi");
        m_config.setPassphrase("helloworld123");
        m_config.setProtocol("WPA");
        m_manager = QWifiManager::instance();
        if (m_manager->backendState() == QWifiManager::Running) {
            m_manager->connect(&m_config);
        } else {
            connect(m_manager, &QWifiManager::backendStateChanged,
                    this, &WifiConnectionHandler::connectToNetwork);
            m_manager->start();
        }
    }

protected slots:
    void connectToNetwork(QWifiManager::BackendState state)
    {
        if (state == QWifiManager::Running)
            m_manager->connect(&m_config);
    }

private:
    QWifiManager *m_manager;
    QWifiConfiguration m_config;
};
//! [3]

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WifiSettings wifiSettingsWindow;
    wifiSettingsWindow.show();

    // disable the above 2 lines before enabling this
    // WifiConnectionHandler connectionHandler;

    return a.exec();
}

#include "main.moc"
