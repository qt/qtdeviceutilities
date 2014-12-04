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
#include "mainwindow.h" // Page_ enum

extern QString G_platform;
extern QString G_version;
extern QString G_os;
extern QString G_device;
extern QString G_board;
extern QString G_SDKDIR;

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
    QStringList data = buttonData();
    if (data.isEmpty())
        return false;

    if (data[0] == "nexus7") {
       mError->setText(tr("The selected platform is not supported."));
       return false;
    }
    if (data[0] == "iMX6" && data[1] == "eAndroid") {
       mError->setText(tr("The selected platform is not supported."));
       return false;
    }

    return !data.isEmpty();
}

void PlatformPage::itemSelected()
{
    mError->clear();
    emit completeChanged();
}

void PlatformPage::initializePage()
{
    mError->clear();

    qDeleteAll(mButtons);
    mButtons.clear();
    mButtonData.clear();

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

            if (os == "eAndroid" && name.startsWith("generic-")) {
                QString version = token[1];
                QDir dir3(dir2.absoluteFilePath(j) + "/images");
                foreach (const QString k, dir3.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
                    if (k == "common")
                        continue;

                    if (!QFile::exists(dir3.absoluteFilePath(k) + "/deploy.sh"))
                        continue;

                    QRadioButton *button = new QRadioButton;
                    button->setText(k + "-" + version + "-" + os + " (" + i + ")");
                    mLayout->insertWidget(0, button);
                    connect(button, &QRadioButton::toggled, this, &PlatformPage::itemSelected);
                    mButtons.append(button);
                    mButtonData.insert(button, QStringList() << name << os << i << k);
                }

            } else if (os == "eAndroid" || os == "eLinux") {
                // Will be fixed with http://qt-gerrit.it.local/#change,7842
                if (name == "imx6qsabresd") {
                  {
                    QRadioButton *button = new QRadioButton;
                    button->setText(j + " (" + i + ")");
                    mLayout->insertWidget(0, button);
                    connect(button, &QRadioButton::toggled, this, &PlatformPage::itemSelected);
                    mButtons.append(button);
                    mButtonData.insert(button, QStringList() << name << os << i << name);
                  }
                  {
                    QRadioButton *button = new QRadioButton;
                    button->setText("imx6dsabresd-eLinux (" + i + ")");
                    mLayout->insertWidget(0, button);
                    connect(button, &QRadioButton::toggled, this, &PlatformPage::itemSelected);
                    mButtons.append(button);
                    mButtonData.insert(button, QStringList() << "imx6dsabresd" << os << i << name);
                  }
                } else {
                  QRadioButton *button = new QRadioButton;
                  button->setText(j + " (" + i + ")");
                  mLayout->insertWidget(0, button);
                  connect(button, &QRadioButton::toggled, this, &PlatformPage::itemSelected);
                  mButtons.append(button);
                  mButtonData.insert(button, QStringList() << name << os << i << name);
                }
            }
        }
    }
    if (mButtons.isEmpty()) {
        mError->setText(tr("No suitable platform found in '%1'.\nMake sure you have"
                           "installed at least one hardware platform.").arg(G_SDKDIR));
    }
}

QStringList PlatformPage::buttonData() const
{
    QStringList data;

    foreach (QRadioButton *button, mButtons) {
        if (button->isChecked()) {
            data = mButtonData[button];
            break;
        }
    }

    return data;
}

bool PlatformPage::validatePage()
{
    QStringList data = buttonData();

    G_platform = data[0];
    G_os = data[1];
    G_version = data[2];
    G_board = data[3];

    qDebug() << "Selected:" << G_platform << G_os << G_version << G_board;
    return true;
}

int PlatformPage::nextId() const
{
    if (G_board.startsWith("nexus7"))
        return MainWindow::Page_Device;
    else
        return MainWindow::Page_Disk;
}
