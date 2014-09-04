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

#ifndef UPDATE_H
#define UPDATE_H

#include <QObject>
#include <QMap>
class Tar;
class QIODevice;

class Update : public QObject
{
    Q_OBJECT

public:
    Update(QObject *parent = 0);
    virtual ~Update();
    void setDevice(QIODevice *source);

private slots:
    void tarStartingFile(const QString &name);
    void tarEndingFile(const QString &name);
    void tarFinished();

private:
    Tar *mTar;
    QIODevice *mSource;
    QString mCurrentFile;
    QByteArray mCurrentSignature;
    QMap<QString, QString> mMetaInfo;
};

#endif // UPDATE_H
