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

#include "scriptwriter.h"
#include <QPair>
#include <QStringList>
#include <QFileInfo>
#include <QDebug>
#include <QTimer>
#include <QProcessEnvironment>
#include <QElapsedTimer>
#include <sys/stat.h>
#include "elevator.h"
#include "common.h"

ScriptWriter::ScriptWriter(QObject *parent)
    : Actor(parent)
    , mDebug(false)
    , mRoot(false)
{
    mDebug = qEnvironmentVariableIsSet("DEBUG");
}

ScriptWriter::~ScriptWriter()
{
}

void ScriptWriter::setScriptFile(const QString &fileName)
{
    mScriptName = fileName;
}

bool ScriptWriter::ready(QString &error) const
{
    if (mScriptName.isEmpty()) {
        error = tr("File name is empty");
        return false;
    }

    QFileInfo file(mScriptName);
    if (!file.exists()) {
        error = tr("File does not exist: %1").arg(mScriptName);
        return false;
    }
    if (!file.isFile()) {
        error = tr("File %1 is not a file").arg(mScriptName);
        return false;
    }
    if (!file.isReadable()) {
        error = tr("File is not readable");
        return false;
    }
    if (!file.isExecutable()) {
        error = tr("File is not executable");
        return false;
    }
    if (file.size() == 0) {
        error = tr("File is empty");
        return false;
    }

    return true;
}

void ScriptWriter::start()
{
    connect(&mProcess, &QProcess::readyReadStandardOutput, this, &ScriptWriter::readStandardOutput);
    connect(&mProcess, &QProcess::readyReadStandardError, this, &ScriptWriter::readStandardError);
    connect(&mProcess, (void (QProcess::*)(QProcess::ProcessError))&QProcess::error, this, &ScriptWriter::processError);
    connect(&mProcess, (void (QProcess::*)(int, QProcess::ExitStatus))&QProcess::finished, this, &ScriptWriter::processFinished);

    QStringList args;

    if (mRoot)
        args << elevate();
    args << "/bin/sh" << "-x" << mScriptName << mAdditionalArgs;
    qDebug() << "Executing" << args;

    mProcess.setProcessEnvironment(QProcessEnvironment::systemEnvironment());

    mProcess.start(args.takeFirst(), args);
    if (!mProcess.waitForStarted())
        qFatal("Failed to start script");
}

void ScriptWriter::readStandardOutput()
{
    QByteArray ba = mProcess.readAllStandardOutput();
    QList<QByteArray> baList = ba.split('\n');

    foreach (const QByteArray &line, baList) {
        if (line.startsWith("-- STEP -- "))
            emit progress(QString::fromLocal8Bit(line.mid(11).trimmed()));
    }

    emit details(ba);
}

void ScriptWriter::readStandardError()
{
    QByteArray ba = mProcess.readAllStandardError();
    emit errorDetails(ba);
}

void ScriptWriter::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus != QProcess::NormalExit || exitCode != 0) {
        emit failed(tr("Image creation failed"));
        return;
    }
    emit finished();
}

void ScriptWriter::processError(QProcess::ProcessError error)
{
    qWarning() << "processError" << error;
}

void ScriptWriter::setEnvironment(const QString &key, const QString &value)
{
    QProcessEnvironment env = mProcess.processEnvironment();
    env.insert(key, value);
    mProcess.setProcessEnvironment(env);
}

void ScriptWriter::setAdditionalArgs(const QStringList &args)
{
    mAdditionalArgs = args;
}

void ScriptWriter::setRootFlag(bool value)
{
    mRoot = value;
}
