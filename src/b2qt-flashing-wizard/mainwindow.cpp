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

#include "mainwindow.h"
#include "introduction_page.h"
#include "platform_page.h"
#include "disk_page.h"
#include "device_page.h"
#include "commit_page.h"
#include "progress_page.h"
#include "finish_page.h"
#include <QDebug>
#include <QDir>
#include <QCoreApplication>

QString G_platform; // generic-4.4 / iMX6
QString G_version; // Boot2Qt version
QString G_os; // eAndroid / eLinux
QString G_device; // serial number or SD Card
QString G_board; // nexus7v2,nexus7, etc...
QString G_SDKDIR; // install directory
QString G_mode; // adb or fastboot

MainWindow::MainWindow()
    : QWizard()
{
    setWindowTitle(tr("Boot to Qt Flashing Wizard"));
    setPixmap(QWizard::LogoPixmap, QPixmap(":logo.png").scaled(64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":background.png"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":watermark.png"));
    G_SDKDIR = qgetenv("SDKDIR");
    if (G_SDKDIR.isEmpty())
        G_SDKDIR = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/../..");
    if (!G_SDKDIR.endsWith('/'))
        G_SDKDIR.append('/');

    setWizardStyle(QWizard::ModernStyle);
    setPage(Page_Intro, new IntroductionPage);
    setPage(Page_Platform, new PlatformPage);
    setPage(Page_Disk, new DiskPage);
    setPage(Page_Device, new DevicePage);
    setPage(Page_Commit, new CommitPage);
    setPage(Page_Progress, new ProgressPage);
    setPage(Page_Finish, new FinishPage);
}

MainWindow::~MainWindow()
{
}

