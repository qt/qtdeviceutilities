/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use the contact form at
** http://www.qt.io
**
** This file is part of Qt Enterprise Embedded.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** the contact form at http://www.qt.io
**
****************************************************************************/
#ifndef PERMISSIONCONTROLLER_H
#define PERMISSIONCONTROLLER_H

#include <binder/IPermissionController.h>

class PermissionController : public android::BnPermissionController
{
public:
    static void instantiate();

    bool checkPermission(const android::String16 &permission, int32_t pid, int32_t uid);

private:
    PermissionController();
};

#endif // PERMISSIONCONTROLLER_H
