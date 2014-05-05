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
#include "schedulingpolicyservice.h"

#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <cutils/sched_policy.h>
#include <sched.h>

using namespace android;

enum {
    REQUEST_PRIORITY_TRANSACTION = IBinder::FIRST_CALL_TRANSACTION
};

#define PRIORITY_MIN 1
#define PRIORITY_MAX 3

SchedulingPolicyService::SchedulingPolicyService()
{
}

void SchedulingPolicyService::instantiate()
{
    defaultServiceManager()->addService(String16("scheduling_policy"), new SchedulingPolicyService);
}

status_t SchedulingPolicyService::onTransact(uint32_t code, const Parcel &data, Parcel *reply, uint32_t flags)
{
    switch (code) {
    case REQUEST_PRIORITY_TRANSACTION: {
        CHECK_INTERFACE(ISchedulingPolicyService, data, reply);
        int32_t pid = data.readInt32();
        int32_t tid = data.readInt32();
        int32_t prio = data.readInt32();
        int res = requestPriority_helper(pid, tid, prio);
        reply->writeNoException();
        reply->writeInt32(res);
        return NO_ERROR;
    } break;
    default:
        return BBinder::onTransact(code, data, reply, flags);
    }
}

#if Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR < 3
int SchedulingPolicyService::requestPriority(int32_t pid, int32_t tid, int32_t prio)
#else
int SchedulingPolicyService::requestPriority(int32_t pid, int32_t tid, int32_t prio, bool)
#endif
{
    return requestPriority_helper(pid, tid, prio);
}

int SchedulingPolicyService::requestPriority_helper(int32_t pid, int32_t tid, int32_t prio)
{
    if (prio < PRIORITY_MIN || prio > PRIORITY_MAX)
        return PERMISSION_DENIED;

    int res = set_sched_policy(tid, IPCThreadState::self()->getCallingPid() == pid ? SP_AUDIO_SYS
                                                                                   : SP_AUDIO_APP);
    if (res != NO_ERROR)
        return PERMISSION_DENIED;

    struct sched_param param;
    param.sched_priority = prio;
    res = sched_setscheduler(tid, SCHED_FIFO, &param);
    if (res)
        return PERMISSION_DENIED;

    return OK;
}
