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
#include <QApplication>
#include <QClipboard>
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
    , mToggleDetailsButton(new QPushButton(this))
    , mCopyToClipboardButton(new QPushButton(this))
{
    setTitle("Disk creation");
    setSubTitle("Progress of writing the disk");
    setLayout(new QVBoxLayout(this));
    mProgress->setWordWrap(true);
    mProgress->setText(tr("Starting"));
    layout()->addWidget(mProgress);
    mToggleDetailsButton->setText("Show details");
    layout()->addWidget(mToggleDetailsButton);
    layout()->addWidget(mTextEdit);
    mCopyToClipboardButton->setText("Copy to clipboard");
    layout()->addWidget(mCopyToClipboardButton);

    mTextEdit->hide();
    mCopyToClipboardButton->hide();

    connect(mToggleDetailsButton, &QPushButton::clicked, this, &ProgressPage::toggleDetails);
    connect(mCopyToClipboardButton, &QPushButton::clicked, this, &ProgressPage::copyDetailsToClipboard);
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

void ProgressPage::failed(const QString &message)
{
    mFinished = false;
    emit completeChanged();
    mProgress->setText(message);
}

void ProgressPage::setActor(Actor *actor)
{
    Q_ASSERT(actor);
    mActor = actor;
    connect(actor, &Actor::finished, this, &ProgressPage::finished);
    connect(actor, &Actor::failed, this, &ProgressPage::failed);
    connect(actor, &Actor::details, this, &ProgressPage::addDetails);
    connect(actor, &Actor::progress, this, &ProgressPage::progress);
}

void ProgressPage::addDetails(QByteArray newData)
{
    newData.replace(0x08 /* backspace */, ' ');
    mTextEdit->append(QString::fromLocal8Bit(newData));
}

void ProgressPage::toggleDetails()
{
    if (mTextEdit->isHidden()) {
        mTextEdit->show();
        mCopyToClipboardButton->show();
        mToggleDetailsButton->setText("Hide details");
    } else {
        mTextEdit->hide();
        mCopyToClipboardButton->hide();
        mToggleDetailsButton->setText("Show details");
    }
}

void ProgressPage::copyDetailsToClipboard()
{
    QApplication::clipboard()->setText(mTextEdit->toPlainText());
}
