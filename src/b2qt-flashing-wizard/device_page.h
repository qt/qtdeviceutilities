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

#ifndef DEVICE_PAGE_H
#define DEVICE_PAGE_H

#include <QWizardPage>
#include <QMap>
class QListWidget;
class QLabel;
class QVBoxLayout;
class QListWidgetItem;

class DevicePage : public QWizardPage
{
    Q_OBJECT
public:
    DevicePage(QWidget *parent = 0);
    virtual ~DevicePage();
    bool isComplete() const;
    void initializePage();
    bool validatePage();
    int nextId() const;

private slots:
    void itemSelected();
    void updateDeviceList();

private:
    struct DeviceInfo
    {
        QString name;
        QString serial;
        QString mode;
        QString state;
    };
    QList<DeviceInfo> list() const;

private:
    QListWidget *mListWidget;
    QMap<QString, QListWidgetItem*> mListItems;
    QMap<QString, DeviceInfo> mDeviceInfo;
    QLabel *mError;
    QVBoxLayout *mLayout;
};

#endif // DEVICE_PAGE_H
