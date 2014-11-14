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

#ifndef POWERMANAGER_H
#define POWERMANAGER_H

#include <powermanager/IPowerManager.h>

namespace android {

typedef BnInterface<IPowerManager> BnPowerManagerService;

} // namespace android

class PowerManager : public android::BnPowerManagerService
{
public:
    static void instantiate();
    android::status_t onTransact(uint32_t code,
                                 const android::Parcel &data,
                                 android::Parcel *reply,
                                 uint32_t flags);

#if Q_ANDROID_VERSION_MAJOR > 4 || (Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR >= 4)
    virtual android::status_t acquireWakeLock(int flags,
                                              const android::sp<android::IBinder>& lock,
                                              const android::String16& tag,
                                              const android::String16& packageName);
    virtual android::status_t acquireWakeLockWithUid(int flags,
                                                     const android::sp<android::IBinder>& lock,
                                                     const android::String16& tag,
                                                     const android::String16& packageName,
                                                     int uid);
    virtual android::status_t updateWakeLockUids(const android::sp<android::IBinder>& lock,
                                                 int len,
                                                 const int *uids);
#else // < 4.4
    virtual android::status_t acquireWakeLock(int flags,
                                              const android::sp<android::IBinder>& lock,
                                              const android::String16& tag);
#endif

    virtual android::status_t releaseWakeLock(const android::sp<android::IBinder>& lock, int flags);
};

#endif // POWERMANAGER_H
