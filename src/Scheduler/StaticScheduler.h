//
// Created by moritz on 17.06.19.
//

#ifndef SCHEDULER_STATICSCHEDULER_H
#define SCHEDULER_STATICSCHEDULER_H

#include "Scheduler.h"

namespace SCHEDULER {
    class __declspec(dllexport) StaticScheduler : public Scheduler{
    public:
        StaticScheduler()= delete;
        StaticScheduler(std::vector<Task> tasks, std::vector<Device> device);
        void schedule() override;

    private:
    };
}

#endif //SCHEDULER_STATICSCHEDULER_H
