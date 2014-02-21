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
#ifndef OMXPLAYER_H
#define OMXPLAYER_H

#include <QObject>

class OmxPlayer : public QObject
{
    Q_OBJECT
public:
    virtual ~OmxPlayer() = 0;

    virtual void updateTexture() = 0;
    virtual bool initialize(const QByteArray &filename) = 0;
    virtual bool hasFrame() = 0;
    virtual void setPaused(bool paused) = 0;

    static OmxPlayer *create();

signals:
    void frameAvailable();
    void videoSize(int w, int h);
};

#endif
