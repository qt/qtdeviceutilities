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
