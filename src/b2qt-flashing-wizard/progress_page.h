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
class QProgressBar;
class Actor;
class QTextEdit;

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
    void progress(unsigned int, unsigned int, unsigned int);
    void finished();
    void addDetails(QByteArray newData);

private slots:
    void progressTimeout();

private:
    QProgressBar* mProgress;
    Actor *mActor;
    bool mFinished;
    QTextEdit *mTextEdit;
    int mNextProgress;
    QTimer *mTimer;
};

#endif // PROGRESS_PAGE_H
