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

#include "device_page.h"
#include "common.h"
#include "mainwindow.h" // for Page_ enum
#include <QRadioButton>
#include <QLayout>
#include <QDebug>
#include <QDir>
#include <QListWidget>
#include <QLabel>
#include <QProcess>
#include <QTimer>

extern QString G_SDKDIR;
extern QString G_device;
extern QString G_board;
extern QString G_mode;

QLabel *createErrorLabel(QWidget *parent);

DevicePage::DevicePage(QWidget *parent)
    : QWizardPage(parent)
    , mListWidget(new QListWidget(this))
    , mError(createErrorLabel(this))
    , mLayout(new QVBoxLayout(this))
{
    setTitle("Device selection");
    setSubTitle("Select a device to be used");
    mLayout->addWidget(mListWidget);
    mLayout->addSpacerItem(new QSpacerItem(40,40,QSizePolicy::Minimum, QSizePolicy::Expanding));
    mLayout->addWidget(mError);
    setLayout(mLayout);
    connect(mListWidget, &QListWidget::itemClicked, this, &DevicePage::itemSelected);

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &DevicePage::updateDeviceList);
    timer->start();
}

DevicePage::~DevicePage()
{
}

QList<DevicePage::DeviceInfo> DevicePage::list() const
{
    QList<DeviceInfo> diList;

    QProcess process;
    process.start(G_SDKDIR + "/Tools/b2qt/adb", QStringList() << "devices" << "-l");
    process.waitForFinished();

    QList<QByteArray> lines = process.readAll().split('\n');
    foreach (const QByteArray &ba, lines) {
        if (ba.startsWith("List of"))
            continue;
        if (ba.isEmpty())
            continue;
        if (ba.startsWith("192.168.56.101:"))
            continue;
        QList<QByteArray> token = ba.simplified().split(' ');

        if (token.size() == 6){
            DeviceInfo d;

            if (token[5].startsWith("device:")) {
                QString deviceName = token[5].mid(7);
                if (deviceName == "grouper")
                    d.name = "Nexus7";
                else if (deviceName == "flo")
                    d.name = "Nexus7v2";
                else
                    d.name = deviceName;
            }
            d.serial = token[0];
            d.mode = "adb";
            if (token[1] == "device")
                d.state = "ready";
            else
                d.state = "not ready";
            diList.append(d);
        } else if (token.size() == 3) {
            DeviceInfo d;
            d.serial = token[0];
            d.mode = "adb";
            d.name = "Unknown device";
            if (token[1] == "offline")
                d.state = "not ready";
            else {
                qDebug() << "Unknown state:" << token[1];
                continue;
            }
            diList.append(d);
        } else {
            qDebug() << "Invalid token count:" << token.size() << token;
            continue;
        }

    }

    process.start(G_SDKDIR + "/Tools/b2qt/fastboot", QStringList("devices") << "-l");
    process.waitForFinished();
    lines = process.readAll().split('\n');
    foreach (const QByteArray &ba, lines) {
        if (ba.isEmpty())
            continue;

        QList<QByteArray> token = ba.simplified().split(' ');

        if (token.size() != 3) {
            qDebug() << "Invalid token count:" << token;
            continue;
        }


        DeviceInfo d;
        d.serial = token[0];

        QProcess p2;
        p2.start(G_SDKDIR + "/Tools/b2qt/fastboot", QStringList() << "-s" << d.serial << "getvar" << "product");
        if (!p2.waitForFinished()) {
            qDebug() << "Could not get product type";
            d.name = "Unknown";
        }

        QString productString = p2.readAllStandardError().split('\n')[0].simplified();
        if (productString.startsWith("product: ")) {
            QString productName = productString.mid(9);
            if (productName == "grouper")
                d.name = "Nexus7";
            else if (productName == "flo")
                d.name = "Nexus7v2";
            else
                d.name = "Unknown";
        } else
            d.name = "Unknown";

        d.mode = "fastboot";
        d.state = "ready";
        diList.append(d);
    }

    return diList;
}

bool DevicePage::isComplete() const
{
    int index = mListWidget->currentRow();
    if (index < 0 || index >= mListItems.size()) {
        return false;
    }

    return true;
}

void DevicePage::itemSelected()
{
    emit completeChanged();
}

void DevicePage::updateDeviceList()
{
    if (wizard()->currentPage() != this)
        return;

    QList<DeviceInfo> diList = list();

    QStringList currentDevices;

    foreach (const DeviceInfo &di, diList)
        currentDevices += di.serial;

    QMutableMapIterator<QString, QListWidgetItem*> iter(mListItems);
    while (iter.hasNext()) {
        iter.next();
        if (!currentDevices.contains(iter.key())) {
            mDeviceInfo.remove(iter.key());
            delete iter.value();
            iter.remove();
        }
    }

    foreach (const DeviceInfo &di, diList) {
        QString text = "<h3>%1</h3><table style=\"margin-left:10px\"><tr><td>Serial number</td><td style=\"padding-left:10\">%2</td></tr><tr><td>Mode</td><td style=\"padding-left:10\">%3</td></tr><tr><td>State</td><td style=\"padding-left:10\">%4</td></tr></table>";
        text = text.arg(di.name, di.serial, di.mode, di.state);

        mDeviceInfo[di.serial] = di;
        if (mListItems.contains(di.serial)) {
            // update existing item
            QListWidgetItem *item = mListItems.value(di.serial);
            QLabel *label = qobject_cast<QLabel*>(mListWidget->itemWidget(item));
            label->setText(text);
        } else {
            // new item
            QListWidgetItem *item = new QListWidgetItem();
            item->setData(Qt::UserRole, di.serial);
            QLabel *label = new QLabel;
            label->setText(text);
            mListWidget->addItem(item);
            mListWidget->setItemWidget(item, label);
            item->setSizeHint(label->size());
            mListItems.insert(di.serial, item);
        }
    }

    if (mListWidget->count() == 0)
        mError->setText("No suitable device found");
    else
        mError->setText("");

    emit completeChanged();
}


void DevicePage::initializePage()
{
    mError->clear();
    updateDeviceList();
}

bool DevicePage::validatePage()
{
    QListWidgetItem *item = mListWidget->currentItem();
    if (!item) {
        return false;
    }

    QString serial = item->data(Qt::UserRole).toString();
    if (serial.isEmpty()) {
        return false;
    }

    if (!mDeviceInfo.contains(serial)) {
        return false;
    }

    DeviceInfo deviceInfo = mDeviceInfo[serial];
    if (deviceInfo.state != "ready") {
        return false;
    }

    G_device = deviceInfo.serial;
    G_board  = deviceInfo.name.toLower();
    G_mode = deviceInfo.mode;
    return true;
}

int DevicePage::nextId() const
{
    return MainWindow::Page_Commit;
}
