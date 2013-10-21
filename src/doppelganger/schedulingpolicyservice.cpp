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
        int res = requestPriority(pid, tid, prio);
        reply->writeNoException();
        reply->writeInt32(res);
        return NO_ERROR;
    } break;
    default:
        return BBinder::onTransact(code, data, reply, flags);
    }
}

int SchedulingPolicyService::requestPriority(int32_t pid, int32_t tid, int32_t prio)
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
