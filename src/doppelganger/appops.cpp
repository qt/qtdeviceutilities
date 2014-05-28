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

#include "appops.h"

#if Q_ANDROID_VERSION_MAJOR > 4 || (Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR >= 4)
#include <binder/IServiceManager.h>
#include <binder/IAppOpsCallback.h>

class AppOpsPrivate : public android::BnAppOpsCallback
{
public:
    virtual void opChanged(int32_t, const android::String16&)
    {

    }

    virtual android::status_t onTransact(uint32_t,
                                         const android::Parcel&,
                                         android::Parcel*,
                                         uint32_t flags = 0)
    {
        (void)flags;
        return android::OK;
    }
};
#endif

void AppOps::instantiate()
{
#if Q_ANDROID_VERSION_MAJOR > 4 || (Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR >= 4)
    android::defaultServiceManager()->addService(android::String16("appops"), new AppOpsPrivate);
#endif
}
