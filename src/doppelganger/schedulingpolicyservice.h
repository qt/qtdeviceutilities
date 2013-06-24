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

    int requestPriority(int32_t pid, int32_t tid, int32_t prio);

private:
    SchedulingPolicyService();
};

#endif // SCHEDULINGPOLICYSERVICE_H
