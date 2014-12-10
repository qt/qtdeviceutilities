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

#ifndef PLATFORMINFO_H
#define PLATFORMINFO_H

#include <QStringList>

class PlatformInfo {
public:
      PlatformInfo() : asroot(false) { }
      QString name; // Display name
      QString platform; // generic-4.4 / iMX6
      QString os; // eAndroid / eLinux
      QString board; // nexus7v2,nexus7, etc...
      QString deployCommand; // script
      QStringList deployArguments;
      QString androidversion;
      QString version; // Boot2Qt version
      bool asroot; // script needs root privileges
};

#endif // PLATFORMINFO_H
