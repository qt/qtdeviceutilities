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

#include "platform_page.h"
#include <QRadioButton>
#include <QLayout>
#include <QDebug>
#include <QDir>
#include <QLabel>
#include <QSpacerItem>
#include <QSettings>
#include "mainwindow.h" // Page_ enum

static QList<PlatformInfo> platforms;
extern QString G_SDKDIR;
extern PlatformInfo G_platforminfo;

QLabel *createErrorLabel(QWidget *parent)
{
    QLabel *label = new QLabel(parent);
    label->setAlignment(Qt::AlignHCenter);
    label->setWordWrap(true);

    QFont f = label->font();
    f.setBold(true);
    label->setFont(f);

    QPalette p = label->palette();
    p.setColor(QPalette::WindowText, Qt::red);
    label->setPalette(p);

    return label;
}

PlatformPage::PlatformPage(QWidget *parent)
    : QWizardPage(parent)
    , mError(createErrorLabel(this))
    , mLayout(new QVBoxLayout(this))
{
    setTitle(tr("Platform"));
    setSubTitle(tr("Select a platform to create a disk for"));
    mLayout->addSpacerItem(new QSpacerItem(40,40,QSizePolicy::Minimum, QSizePolicy::Expanding));
    mLayout->addWidget(mError);
    setLayout(mLayout);
}

PlatformPage::~PlatformPage()
{
}

bool PlatformPage::isComplete() const
{
    if (mButtons.isEmpty() || buttonData().name.isEmpty())
          return false;
    return true;
}

void PlatformPage::itemSelected()
{
    mError->clear();
    emit completeChanged();
}

static void loadDeployConfig(const QString &filename, const QString &version)
{
    qDebug() << "Trying to load config" << filename;
    if (!QFile::exists(filename))
        return;

    QFileInfo fi(filename);

    QSettings settings(filename, QSettings::IniFormat);

    foreach (const QString &group, settings.childGroups()) {
        PlatformInfo pi;

        settings.beginGroup(group);
        pi.name = group;
        pi.platform = settings.value("platform").toString();
        pi.os = settings.value("os").toString();
        pi.androidversion = settings.value("androidversion").toString();
        pi.board = settings.value("board").toString();
        pi.deployCommand = settings.value("deploycommand").toString();
        pi.deployCommand = QDir::cleanPath(fi.canonicalPath() + "/" + pi.deployCommand);
        pi.deployArguments = settings.value("deployarguments").toStringList();
        pi.asroot = settings.value("asroot").toBool();
        pi.version = version;
        settings.endGroup();

        if (pi.platform.isEmpty() || pi.os.isEmpty() || pi.deployCommand.isEmpty()) {
            qWarning() << "Invalid data";
            continue;
        }

        if (pi.os == "eAndroid" && pi.androidversion.isEmpty()) {
            qWarning() << "Invalid data";
            continue;
        }

        qDebug() << "Adding platform" << group;
        platforms.append(pi);
    }
}

void PlatformPage::initializePage()
{
    mError->clear();

    qDeleteAll(mButtons);
    mButtons.clear();

    QDir dir(G_SDKDIR);
    foreach (const QString i, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (!i.startsWith("Boot2Qt-"))
          continue;

        QDir dir2(dir.absoluteFilePath(i));
        foreach (const QString j, dir2.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            if (j.startsWith("emulator-"))
                continue;
            QStringList token = j.split('-');
            QString os = token.takeLast();
            QString name = token.join("-");

            loadDeployConfig(dir2.absoluteFilePath(j) + "/images/deploy.conf", i);
        }
    }
    if (platforms.isEmpty()) {
        mError->setText(tr("No suitable platform found in '%1'.\nMake sure you have"
                           "installed at least one hardware platform.").arg(G_SDKDIR));
    }

    for (int i = 0; i < platforms.count(); ++i) {
          const PlatformInfo &pi = platforms[i];
          QRadioButton *button = new QRadioButton;
          if (pi.os == "eAndroid")
              button->setText(pi.name + " | " + pi.os + " " + pi.androidversion + " (" + pi.version + ")");
          else
              button->setText(pi.name + " | " + pi.os + " (" + pi.version + ")");
          mLayout->insertWidget(0, button);
          connect(button, &QRadioButton::toggled, this, &PlatformPage::itemSelected);
          button->setProperty("b2qt-platform-name", i);
          mButtons.append(button);
    }
}

PlatformInfo PlatformPage::buttonData() const
{
    foreach (QRadioButton *button, mButtons) {
        if (button->isChecked()) {
            int id = button->property("b2qt-platform-name").toInt();
            return platforms[id];
        }
    }

    return PlatformInfo();
}

bool PlatformPage::validatePage()
{
    G_platforminfo = buttonData();
    qDebug() << "Selected:" << G_platforminfo.name;
    return true;
}

int PlatformPage::nextId() const
{
    if (G_platforminfo.board.startsWith("nexus7"))
        return MainWindow::Page_Device;
    else
        return MainWindow::Page_Disk;
}

