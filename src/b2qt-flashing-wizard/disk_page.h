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

#ifndef DISK_PAGE_H
#define DISK_PAGE_H

#include <QWizardPage>
#include <QMap>
#include <QSet>
class QListWidget;
class QLabel;
class QVBoxLayout;
class QListWidgetItem;

class DiskPage : public QWizardPage
{
    Q_OBJECT
public:
    DiskPage(QWidget *parent = 0);
    virtual ~DiskPage();
    bool isComplete() const;
    void initializePage();
    bool validatePage();
    int nextId() const;

private slots:
    void itemSelected();
    void updateDeviceList();

private:
    struct DiskInfo
    {
        QString path;
        bool removable;
        QString name;
        quint64 logicalBlockSize;
        quint64 blocks;
        QString mountPoint;
        QString mountPartition;
    };
    QList<DiskInfo> list() const;

private:
    QListWidget *mListWidget;
    QMap<QString, QListWidgetItem*> mListItems;
    QMap<QString, DiskInfo> mDiskInfo;
    QLabel *mError;
    QVBoxLayout *mLayout;
    QSet<QString> mHiddenDevices;
};

#endif // DISK_PAGE_H
