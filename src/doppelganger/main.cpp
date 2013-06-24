#include <binder/IPCThreadState.h>

#include "permissioncontroller.h"
#include "schedulingpolicyservice.h"

using namespace android;

int main(int, char *[])
{
    sp<ProcessState> proc(ProcessState::self());
    SchedulingPolicyService::instantiate();
    PermissionController::instantiate();
    IPCThreadState::self()->joinThreadPool();
}
