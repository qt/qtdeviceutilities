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
#include <QApplication>
//#include <QCleanlooksStyle>
#include <QStyleFactory>
#include <QDebug>
#include <QIcon>

int main(int argc, char **argv)
{
    printf("Version %s, SHA1 %s\n", GIT_VERSION, GIT_HASH);
    QApplication app(argc, argv);
//    qDebug() << QStyleFactory::keys();
    QStyle *style = QStyleFactory::create("Fusion");
    if (style)
        app.setStyle(style);
    app.setWindowIcon(QIcon(":logo.png"));
    MainWindow mw;
    mw.show();
    return app.exec();
}
