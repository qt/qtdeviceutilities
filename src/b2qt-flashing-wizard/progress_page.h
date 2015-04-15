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

#ifndef PROGRESS_PAGE_H
#define PROGRESS_PAGE_H

#include <QWizardPage>
class QLabel;
class Actor;
class QTextEdit;
class QPushButton;
class QProgressBar;

class ProgressPage : public QWizardPage
{
    Q_OBJECT
public:
    ProgressPage(QWidget *parent = 0);
    virtual ~ProgressPage();
    bool isComplete() const;
    void initializePage();
    void setActor(Actor *actor);

public slots:
    void progress(const QString &step);
    void finished();
    void failed(const QString &step);
    void addDetails(QByteArray newData);
    void addErrorDetails(QByteArray newData);
    void showDetails();
    void hideDetails();
    void toggleDetails();
    void copyDetailsToClipboard();

private:
    QLabel *mProgress;
    Actor *mActor;
    bool mFinished;
    QTextEdit *mTextEdit;
    QPushButton *mToggleDetailsButton;
    QPushButton *mCopyToClipboardButton;
    QProgressBar *mProgressBar;
};

#endif // PROGRESS_PAGE_H
