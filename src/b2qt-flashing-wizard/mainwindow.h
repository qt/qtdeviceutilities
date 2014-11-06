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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWizard>

class MainWindow : public QWizard
{
    Q_OBJECT

public:
      enum {Page_Intro, Page_Platform, Page_Disk, Page_Device, Page_Commit, Page_Progress, Page_Finish};
      MainWindow();
      virtual ~MainWindow();
};

#endif // MAINWINDOW_H
