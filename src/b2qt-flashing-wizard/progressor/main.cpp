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

#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QElapsedTimer>
#include <QSocketNotifier>
#include <QFile>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QStringList args = app.arguments();

    args.removeFirst();
    if (args.size() == 0) {
        qDebug() << "Usage: Program to excute";
        return 1;
    }

    QList<qint32> timings;
    QElapsedTimer timer;
    QProcess p;
    p.setProcessChannelMode(QProcess::MergedChannels);
    QSocketNotifier notifier(0, QSocketNotifier::Read);
    QFile standardInput;
    if (!standardInput.open(0, QFile::ReadOnly))
        qFatal("open");
    QObject::connect(&notifier, &QSocketNotifier::activated, [&p, &standardInput](){ p.write(standardInput.readAll());});

    QObject::connect(&p, (void (QProcess::*)(int, QProcess::ExitStatus))&QProcess::finished, [&timings, &timer](int exitCode, QProcess::ExitStatus exitStatus) {
        timings += -timer.elapsed();
        if (exitStatus != QProcess::NormalExit || exitCode != 0) {
            qDebug() << "Something went wrong";
            qApp->exit(1);
            return;
        }

        qDebug() << "Doing calculations";
        double factor = 100.0/abs(timings.last());

        for (int i = 0; i < timings.size(); ++i) {
            qint32 value = timings[i];
            qint32 output = value * factor;
            QString text;
            if (i == timings.size()-1)
                fprintf(stdout, "%d\n", value);
            else {
                if (output == 0 && value < 0)
                    fprintf(stdout, "-0\n");
                else
                    fprintf(stdout, "%d\n", output);
            }
        }
        qApp->exit(0);
    });

    QObject::connect(&p, &QProcess::readyRead, [&p, &timer, &timings]() {
          while (p.canReadLine()) {
              QByteArray line = p.readLine();
              line.chop(1);
              qDebug() << line;

              if (line.startsWith("+"))
                  timings += -timer.elapsed();
              else
                  timings += timer.elapsed();
          }
    });

    p.start("/bin/sh", QStringList() << "-x" << args);
    timer.start();
    if (!p.waitForStarted())
        qFatal("Failed to start");

    return app.exec();
}

