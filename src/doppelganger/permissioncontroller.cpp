/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
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
#include "permissioncontroller.h"

#include <binder/IServiceManager.h>

using namespace android;

PermissionController::PermissionController()
{
}

void PermissionController::instantiate()
{
    defaultServiceManager()->addService(String16("permission"), new PermissionController);
}

bool PermissionController::checkPermission(const String16 &, int32_t, int32_t)
{
    // just bypass any permission
    return true;
}
