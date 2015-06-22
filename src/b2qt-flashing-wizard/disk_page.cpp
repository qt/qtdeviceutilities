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

#include "disk_page.h"
#include "common.h"
#include "elevator.h"
#include "mainwindow.h" // for Page_ enum
#include <QRadioButton>
#include <QLayout>
#include <QDebug>
#include <QDir>
#include <QListWidget>
#include <QLabel>
#include <QTimer>
#include <QProcess>
#include <QMessageBox>

extern QString G_device;
QLabel *createErrorLabel(QWidget *parent);

DiskPage::DiskPage(QWidget *parent)
    : QWizardPage(parent)
    , mListWidget(new QListWidget(this))
    , mError(createErrorLabel(this))
    , mLayout(new QVBoxLayout(this))
{
    setTitle(tr("Disk"));
    setSubTitle(tr("Select a disk to be used"));
    mLayout->addWidget(mListWidget);
    mLayout->addSpacerItem(new QSpacerItem(40,40,QSizePolicy::Minimum, QSizePolicy::Expanding));
    mLayout->addWidget(mError);
    setLayout(mLayout);
    connect(mListWidget, &QListWidget::itemClicked, this, &DiskPage::itemSelected);

    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, &QTimer::timeout, this, &DiskPage::updateDeviceList);
    timer->start();

    // Remember devices already present at startup
    foreach (const DiskInfo &di, list()) {
        if (!di.removable)
            mHiddenDevices += di.path;
    }
}

DiskPage::~DiskPage()
{
}

QList<DiskPage::DiskInfo> DiskPage::list() const
{
    QList<DiskInfo> diList;

    QDir sys("/sys/block");

    foreach (const QString &i, sys.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        DiskInfo di;

        if (readAll(sys.absoluteFilePath(i) + "/removable").trimmed() == "1")
            di.removable = true;
        else
            di.removable = false;

        di.logicalBlockSize = readAll(sys.absoluteFilePath(i) + "/queue/logical_block_size").trimmed().toULong();
        di.blocks = readAll(sys.absoluteFilePath(i) + "/size").trimmed().toULong();
        di.name = i;
        di.path = "/dev/" + i;

        checkForDeviceMounted(di.path, di.mountPoint, di.mountPartition);

        diList.append(di);
    }

    return diList;
}

bool DiskPage::isComplete() const
{
    QListWidgetItem *item = mListWidget->currentItem();
    if (!item)
        return false;

    QString path = item->data(Qt::UserRole).toString();
    if (path.isEmpty())
        return false;
    if (!mListItems.contains(path))
        return false;

    return true;
}

void DiskPage::itemSelected()
{
    emit completeChanged();
}

void DiskPage::updateDeviceList()
{
    if (wizard()->currentPage() != this)
        return;

    QList<DiskInfo> diList = list();

    QSet<QString> currentDevices;

    {   // filter devices by name, size etc.
        QMutableListIterator<DiskInfo> iter(diList);

        while (iter.hasNext()) {
            iter.next();
            const DiskInfo &di = iter.value();

            if (di.logicalBlockSize == 0 || di.blocks == 0 || di.name.startsWith("ram")
                || di.name.startsWith("sr") || di.name.startsWith("fd")) {
                iter.remove();
                continue;
            }
            currentDevices += di.path;
        }
    }

    { // Update hidden devices list if a device was removed
        QMutableSetIterator<QString> iter(mHiddenDevices);

        while (iter.hasNext()) {
            iter.next();
            if (!currentDevices.contains(iter.value()))
                  iter.remove();
        }
    }
    currentDevices.subtract(mHiddenDevices);

    // Remove devices from map and UI
    QMutableMapIterator<QString, QListWidgetItem*> iter(mListItems);
    while (iter.hasNext()) {
        iter.next();
        if (!currentDevices.contains(iter.key())) {
            mDiskInfo.remove(iter.key());
            delete iter.value();
            iter.remove();
        }
    }

    foreach (const DiskInfo &di, diList) {
        if (!currentDevices.contains(di.path))
              continue;

        QString text = tr("<h3>%1</h3><table style=\"margin-left:10px\"><tr><td>Size</td>"
          "<td style=\"padding-left:10\">%2</td></tr><tr><td>Removable</td>"
          "<td style=\"padding-left:10\">%3</td><tr><td>Mounted</td>"
          "<td style=\"padding-left:10\">%4</td></tr></table>");
        double size = di.logicalBlockSize * di.blocks;
        QString sizeText;

        if (size < 1000) {
            sizeText = QString::number(size) + tr(" B");
        } else if (size < 1000000) {
            sizeText = QString::number(size / 1000) + tr(" KB");
        } else if (size < 1000000000) {
            sizeText = QString::number(size / 1000000) + tr(" MB");
        } else {
            sizeText = QString::number(qRound(size / 1000 / 1000 / 1000)) + tr(" GB");
        }

        text = text.arg(di.name, sizeText, di.removable?tr("yes"):tr("no"), di.mountPoint.isEmpty()?tr("no"):di.mountPoint);

        mDiskInfo[di.path] = di;
        if (mListItems.contains(di.path)) {
            // update existing item
            QListWidgetItem *item = mListItems.value(di.path);
            QLabel *label = qobject_cast<QLabel*>(mListWidget->itemWidget(item));
            label->setText(text);
        } else {
            // new item
            QListWidgetItem *item = new QListWidgetItem();
            item->setData(Qt::UserRole, di.path);
            QLabel *label = new QLabel;
            label->setText(text);
            mListWidget->addItem(item);
            mListWidget->setItemWidget(item, label);
            item->setSizeHint(label->size());
            mListItems.insert(di.path, item);
        }
    }

    if (mListWidget->count() == 0)
        mError->setText(tr("No suitable disk device found. Connect a device."));
    else
        mError->setText("");

    emit completeChanged();
}

void DiskPage::initializePage()
{
    mError->clear();
    updateDeviceList();
}

bool DiskPage::validatePage()
{
    QListWidgetItem *item = mListWidget->currentItem();
    if (!item)
        return false;
    QString path = item->data(Qt::UserRole).toString();
    if (path.isEmpty())
        return false;

    DiskInfo di = mDiskInfo[path];

    if (!di.mountPoint.isEmpty()) {
        // Ask the user before unmount
        if (QMessageBox::Ok != QMessageBox::warning(this, tr("Unmount action"), tr("The disk you selected is mounted at '%1'. It will be unmounted now.").arg(di.mountPoint), QMessageBox::Ok | QMessageBox::Cancel))
            return false;

        QStringList args = elevate();
        args << "umount" << di.mountPoint;

        int rc = QProcess::execute(args.takeFirst(), args);
        if (rc != 0) {
            QMessageBox::critical(this, tr("Unmount failed"), tr("Could not umount the disk."));
            return false;
        }
    }

    G_device = path;
    return true;
}

int DiskPage::nextId() const
{
    return MainWindow::Page_Commit;
}
