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

#ifndef ACTOR_H
#define ACTOR_H

#include <QObject>

class Actor : public QObject
{
    Q_OBJECT

public:
    Actor(QObject *parent);
    virtual ~Actor();
    virtual bool ready(QString &) const = 0;
    virtual void start() = 0;

signals:
    void progress(unsigned int percent, unsigned int nextProgress, unsigned int milliseconds);
    void finished();
    void details(const QByteArray &);
};

#endif // ACTOR_H
