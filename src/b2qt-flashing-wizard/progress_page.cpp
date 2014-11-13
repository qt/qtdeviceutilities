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

#include "progress_page.h"
#include "actor.h"
#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>

ProgressPage::ProgressPage(QWidget *parent)
    : QWizardPage(parent)
    , mProgress(new QLabel(this))
    , mActor(0)
    , mFinished(false)
    , mTextEdit(new QTextEdit(this))
{
    setTitle("Disk creation");
    setSubTitle("Progress of writing the disk");
    setLayout(new QVBoxLayout(this));
    mProgress->setText(tr("Starting"));
    layout()->addWidget(mProgress);
    QPushButton *button = new QPushButton(this);
    button->setText("Show details");
    layout()->addWidget(button);
    layout()->addWidget(mTextEdit);
    QPushButton *copy = new QPushButton(this);
    copy->setText("Copy to clipboard");
    layout()->addWidget(copy);
}

ProgressPage::~ProgressPage()
{
}

bool ProgressPage::isComplete() const
{
    return mFinished;
}

void ProgressPage::initializePage()
{
    Q_ASSERT(mActor);
    mActor->start();
}

void ProgressPage::progress(const QString &step)
{
    mProgress->setText(step);
}

void ProgressPage::finished()
{
    mFinished = true;
    emit completeChanged();
    wizard()->next(); // progress to next page automatically
}

void ProgressPage::setActor(Actor *actor)
{
    Q_ASSERT(actor);
    mActor = actor;
    connect(actor, &Actor::finished, this, &ProgressPage::finished);
    connect(actor, &Actor::details, this, &ProgressPage::addDetails);
    connect(actor, &Actor::progress, this, &ProgressPage::progress);
}

void ProgressPage::addDetails(QByteArray newData)
{
    newData.replace(0x08 /* backspace */, ' ');
    mTextEdit->append(QString::fromLocal8Bit(newData));
}