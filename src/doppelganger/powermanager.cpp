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

#include "powermanager.h"

#include <binder/IServiceManager.h>

using namespace android;

enum {
    ACQUIRE_WAKE_LOCK = IBinder::FIRST_CALL_TRANSACTION,
    ACQUIRE_WAKE_LOCK_UID = IBinder::FIRST_CALL_TRANSACTION + 1,
    RELEASE_WAKE_LOCK = IBinder::FIRST_CALL_TRANSACTION + 2,
    UPDATE_WAKE_LOCK_UIDS = IBinder::FIRST_CALL_TRANSACTION + 3,
};

void PowerManager::instantiate()
{
    defaultServiceManager()->addService(String16("power"), new PowerManager());
}

status_t PowerManager::onTransact(uint32_t code,
                                  const Parcel &,
                                  Parcel *,
                                  uint32_t)
{
    switch (code) {
    case ACQUIRE_WAKE_LOCK:
    case ACQUIRE_WAKE_LOCK_UID:
    case RELEASE_WAKE_LOCK:
    case UPDATE_WAKE_LOCK_UIDS:
        return NO_ERROR;
    default:
        break;
    }
}

#if Q_ANDROID_VERSION_MAJOR > 4 || (Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR >= 4)
status_t PowerManager::acquireWakeLock(int,
                                       const sp<IBinder> &,
                                       const String16 &,
                                       const String16 &)
{
    return OK;
}

status_t PowerManager::acquireWakeLockWithUid(int,
                                              const sp<IBinder> &,
                                              const String16 &,
                                              const String16 &,
                                              int)
{
    return OK;
}

status_t PowerManager::updateWakeLockUids(const sp<IBinder> &,
                                          int,
                                          const int *)
{
    return OK;
}
#else // < 4.4
status_t PowerManager::acquireWakeLock(int, const sp<android::IBinder> &, const String16 &)
{
    return OK;
}
#endif

status_t PowerManager::releaseWakeLock(const sp<IBinder> &, int)
{
    return OK;
}
