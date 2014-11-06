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

#ifndef FINISH_PAGE_H
#define FINISH_PAGE_H

#include <QWizardPage>

class FinishPage : public QWizardPage
{
    Q_OBJECT

public:
    FinishPage(QWidget *parent = 0);
    virtual ~FinishPage();
};

#endif // FINISH_PAGE_H
