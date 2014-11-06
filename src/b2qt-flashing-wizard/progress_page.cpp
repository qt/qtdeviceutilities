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
#include <QProgressBar>
#include <QLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>

ProgressPage::ProgressPage(QWidget *parent)
    : QWizardPage(parent)
    , mProgress(new QProgressBar(this))
    , mActor(0)
    , mFinished(false)
    , mTextEdit(new QTextEdit(this))
    , mNextProgress(0)
    , mTimer(new QTimer(this))
{
    setTitle("Disk creation");
    setSubTitle("Progress of writing the disk");
    setLayout(new QVBoxLayout(this));
    layout()->addWidget(mProgress);
    mProgress->setMaximum(0);
    mProgress->setMinimum(0);
    QPushButton *button = new QPushButton(this);
    button->setText("Show details");
    layout()->addWidget(button);
    layout()->addWidget(mTextEdit);
    QPushButton *copy = new QPushButton(this);
    copy->setText("Copy to clipboard");
    layout()->addWidget(copy);

    connect(mTimer, &QTimer::timeout, this, &ProgressPage::progressTimeout);
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
    qDebug() << Q_FUNC_INFO;
    Q_ASSERT(mActor);
    mActor->start();
}

void ProgressPage::progressTimeout()
{
    int value = mProgress->value();
    ++value;
    if (value > mNextProgress)
        value = mNextProgress;

    mProgress->setValue(value);
}

void ProgressPage::progress(unsigned int progress, unsigned int nextProgress, unsigned int milliseconds)
{
    if (nextProgress < progress)
        qFatal("Error");

    mNextProgress = nextProgress;
    mProgress->setMaximum(100);

    if (milliseconds && nextProgress != progress) {
        mTimer->setInterval(milliseconds / (nextProgress-progress));
        mTimer->start();
    } else
        mTimer->stop();

    qDebug() << "progress" << progress;
    mProgress->setValue(progress);
}

void ProgressPage::finished()
{
    qDebug() << Q_FUNC_INFO;
    mFinished = true;
    emit completeChanged();
    wizard()->next(); // progress to next page automatically
}

void ProgressPage::setActor(Actor *actor)
{
    Q_ASSERT(actor);
    mActor = actor;
    connect(actor, &Actor::progress, this, &ProgressPage::progress);
    connect(actor, &Actor::finished, this, &ProgressPage::finished);
    connect(actor, &Actor::details, this, &ProgressPage::addDetails);
}

void ProgressPage::addDetails(QByteArray newData)
{
    newData.replace(0x08 /* backspace */, ' ');
    mTextEdit->append(QString::fromLocal8Bit(newData));
}
