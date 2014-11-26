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

#include "commit_page.h"
#include "scriptwriter.h"
#include "progress_page.h"
#include "mainwindow.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>

extern QString G_platform;
extern QString G_version;
extern QString G_os;
extern QString G_device;
extern QString G_board;
extern QString G_SDKDIR;
extern QString G_mode;
QLabel *createErrorLabel(QWidget *parent);

CommitPage::CommitPage(QWidget *parent)
    : QWizardPage(parent)
    , mText(new QLabel(this))
    , mActor(0)
    , mError(createErrorLabel(this))
    , mLayout(new QVBoxLayout(this))
    , mComplete(false)
{
    setButtonText(QWizard::CommitButton, tr("Write"));
    setCommitPage(true);
    setTitle(tr("Confirm Action"));
    setSubTitle(tr("Confirm the action to continue."));
    mLayout->addWidget(mText);
    mLayout->addSpacerItem(new QSpacerItem(40,40,QSizePolicy::Minimum, QSizePolicy::Expanding));
    mLayout->addWidget(mError);
    setLayout(mLayout);
}

CommitPage::~CommitPage()
{
}

bool CommitPage::isComplete() const
{
    if (!mComplete)
        return false;

    QString error;
    bool result = mActor->ready(error);
    if (result)
        mError->clear();
    else
        mError->setText(error);

    return result;
}

void CommitPage::initializePage()
{
    qDebug() << "platform:" << G_platform << "version:" << G_version << "os:" << G_os
             << "device:" << G_device << "board:" << G_board << "mode:" << G_mode;
    QString text(tr("Write %1-%2 (%3) to device %4."));
    mText->setText(text.arg(G_platform, G_os, G_version, G_device));
    mComplete = true;

    qDebug() << "os:" << G_os << "platform:" << G_platform << "device:" << G_device;

    if (G_platform == "generic-4.2" && G_os == "eAndroid" && G_board == "iMX6") {
        ScriptWriter *i = new ScriptWriter(this);
        i->setScriptFile(G_SDKDIR + G_version + "/" + G_platform + "-" + G_os + "/images/" + G_board + "/deploy.sh");
        i->setAdditionalArgs(QStringList() << "-y" <<G_device << "--verbose");
        i->setEnvironment("VERBOSE","1");
        mActor = i;
    } else if (G_platform == "iMX6" && G_os == "eLinux") {
        ScriptWriter *i = new ScriptWriter(this);
        i->setScriptFile(G_SDKDIR + G_version + "/" + G_platform + "-" + G_os + "/images/deploy.sh");
        i->setAdditionalArgs(QStringList() << "-y" <<G_device << "--verbose");
        i->setEnvironment("VERBOSE","1");
        mActor = i;
    } else if (G_platform == "beaglebone" && G_os == "eLinux") {
        ScriptWriter *i = new ScriptWriter(this);
        i->setScriptFile(G_SDKDIR + G_version + "/" + G_platform + "-" + G_os + "/images/deploy.sh");
        i->setAdditionalArgs(QStringList() << "-y" <<G_device << "--verbose");
        i->setEnvironment("VERBOSE","1");
        mActor = i;
    } else if (G_platform == "generic-4.4" && G_os == "eAndroid" && G_board == "beaglebone") {
        ScriptWriter *i = new ScriptWriter(this);
        i->setScriptFile(G_SDKDIR + G_version + "/" + G_platform + "-" + G_os + "/images/" + G_board + "/deploy.sh");
        i->setAdditionalArgs(QStringList() << "-y" <<G_device << "--verbose");
        i->setEnvironment("VERBOSE","1");
        mActor = i;
    } else if (G_platform == "generic-4.4" && G_os == "eAndroid" && G_board == "nexus7v2") {
        ScriptWriter *i = new ScriptWriter(this);
        i->setScriptFile(G_SDKDIR + G_version + "/" + G_platform + "-" + G_os + "/images/" + G_board + "/deploy.sh");
        QStringList args("-y");
        if (G_mode == "fastboot")
            args << "-fastboot";
        i->setAdditionalArgs(args);
        mActor = i;
    } else if (G_platform == "generic-4.4" && G_os == "eAndroid" && G_board == "nexus7") {
        ScriptWriter *i = new ScriptWriter(this);
        i->setScriptFile(G_SDKDIR + G_version + "/" + G_platform + "-" + G_os + "/images/" + G_board + "/deploy.sh");
        QStringList args("-y");
        if (G_mode == "fastboot")
            args << "-fastboot";
        i->setAdditionalArgs(args);
        mActor = i;
    } else if (G_platform == "generic-4.2" && G_os == "eAndroid" && G_board == "nexus7") {
        ScriptWriter *i = new ScriptWriter(this);
        i->setScriptFile(G_SDKDIR + G_version + "/" + G_platform + "-" + G_os + "/images/" + G_board + "/deploy.sh");
        QStringList args("-y");
        if (G_mode == "fastboot")
            args << "-fastboot";
        i->setAdditionalArgs(args);
        mActor = i;
    } else if (G_platform == "raspberrypi" && G_os == "eLinux") {
        ScriptWriter *i = new ScriptWriter(this);
        i->setScriptFile(G_SDKDIR + G_version + "/" + G_platform + "-" + G_os + "/images/deploy.sh");
        i->setAdditionalArgs(QStringList() << "-y" << G_device << "--verbose");
        i->setEnvironment("VERBOSE","1");
        mActor = i;
    } else if (G_platform == "imx6qsabresd" && G_os == "eLinux") {
        ScriptWriter *i = new ScriptWriter(this);
        i->setScriptFile(G_SDKDIR + G_version + "/" + G_platform + "-" + G_os + "/images/deploy.sh");
        i->setAdditionalArgs(QStringList() << "-y" <<G_device << "--verbose");
        i->setEnvironment("VERBOSE","1");
        mActor = i;
    } else if (G_platform == "apalis-imx6" && G_os == "eLinux") {
        ScriptWriter *i = new ScriptWriter(this);
        i->setScriptFile(G_SDKDIR + G_version + "/" + G_platform + "-" + G_os + "/images/deploy.sh");
        i->setAdditionalArgs(QStringList() << "-y" <<G_device << "--verbose");
        i->setEnvironment("VERBOSE","1");
        mActor = i;
    } else if (G_platform == "tibidabo" && G_os == "eLinux") {
        ScriptWriter *i = new ScriptWriter(this);
        i->setScriptFile(G_SDKDIR + G_version + "/" + G_platform + "-" + G_os + "/images/deploy.sh");
        i->setAdditionalArgs(QStringList() << "-y" <<G_device << "--verbose");
        i->setEnvironment("VERBOSE","1");
        mActor = i;
    } else {
        mError->setText(tr("Unsupported platform combination"));
        mComplete = false;
    }
}

bool CommitPage::validatePage()
{
    ProgressPage *p = qobject_cast<ProgressPage*>(wizard()->page(MainWindow::Page_Progress));
    Q_ASSERT(p);

    p->setActor(mActor);
    return true;
}
