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
#include <binder/IPCThreadState.h>

#include "permissioncontroller.h"
#include "schedulingpolicyservice.h"
#include "powermanager.h"

using namespace android;

int main(int, char *[])
{
    sp<ProcessState> proc(ProcessState::self());
    SchedulingPolicyService::instantiate();
    PermissionController::instantiate();
    PowerManager::instantiate();
    IPCThreadState::self()->joinThreadPool();
}
