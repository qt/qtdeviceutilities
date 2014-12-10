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
#include "platforminfo.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>

extern PlatformInfo G_platforminfo;
extern QString G_device;
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
    qDebug() << "platform:" << G_platforminfo.platform << "version:" << G_platforminfo.version
             << "os:" << G_platforminfo.os << "board:" << G_platforminfo.board
             << "deploycommand:" << G_platforminfo.deployCommand << "deployarguments:" << G_platforminfo.deployArguments
             << "device:" << G_device <<  "mode:" << G_mode;

    QString text(tr("Write %1-%2 (%3) to device %4."));
    mText->setText(text.arg(G_platforminfo.platform, G_platforminfo.os, G_platforminfo.version, G_device));
    mComplete = true;

    ScriptWriter *i = new ScriptWriter(this);
    i->setScriptFile(G_platforminfo.deployCommand);
    i->setRootFlag(G_platforminfo.asroot);

    QStringList args;
    foreach (QString a, G_platforminfo.deployArguments) {
        if (a == "@DEVICE@")
            a = G_device;
        args << a;
    }

    if (G_platforminfo.board.startsWith("nexus7") && G_mode == "fastboot")
        args << "-fastboot";

    i->setAdditionalArgs(args);
    i->setEnvironment("VERBOSE","1");
    mActor = i;
}

bool CommitPage::validatePage()
{
    ProgressPage *p = qobject_cast<ProgressPage*>(wizard()->page(MainWindow::Page_Progress));
    Q_ASSERT(p);

    p->setActor(mActor);
    return true;
}
