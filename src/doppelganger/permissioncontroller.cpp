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
