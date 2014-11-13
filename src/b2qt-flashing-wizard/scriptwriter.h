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

#ifndef SCRIPTWRITER_H
#define SCRIPTWRITER_H

#include "actor.h"
#include <QProcess>
class QElapsedTimer;

class ScriptWriter : public Actor
{
    Q_OBJECT

public:
    ScriptWriter(QObject *parent);
    virtual ~ScriptWriter();
    void setScriptFile(const QString &fileName);
    bool ready(QString &) const;
    void start();
    void setEnvironment(const QString &key, const QString &value);
    void setAdditionalArgs(const QStringList &);

private slots:
    void readOutput();
    void processFinished(int, QProcess::ExitStatus);
    void processError(QProcess::ProcessError);

private:
    QString mScriptName;
    QProcess mProcess;
    QStringList mAdditionalArgs;
    bool mDebug;
};

#endif // SCRIPTWRITER_H
