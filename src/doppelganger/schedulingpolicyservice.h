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
#ifndef SCHEDULINGPOLICYSERVICE_H
#define SCHEDULINGPOLICYSERVICE_H

#include <frameworks/av/services/audioflinger/ISchedulingPolicyService.h>
#include <binder/Parcel.h>

class SchedulingPolicyService : public android::BnSchedulingPolicyService
{
public:
    static void instantiate();

    android::status_t onTransact(uint32_t code, const android::Parcel &data,
                                 android::Parcel *reply, uint32_t flags);

#if Q_ANDROID_VERSION_MAJOR == 4 && Q_ANDROID_VERSION_MINOR < 3
    int requestPriority(int32_t pid, int32_t tid, int32_t prio);
#else
    int requestPriority(int32_t pid, int32_t tid, int32_t prio, bool);
#endif

private:
    SchedulingPolicyService();
    int requestPriority_helper(int32_t pid, int32_t tid, int32_t prio);
};

#endif // SCHEDULINGPOLICYSERVICE_H
