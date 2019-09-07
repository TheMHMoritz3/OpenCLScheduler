//
// Created by moritz on 07.09.19.
//

#ifndef UI_NEWSTATICSCHEDULER_H
#define UI_NEWSTATICSCHEDULER_H

#include "Scheduler.h"
#include "Device.h"
#include <vector>
#include "Task.h"

namespace SCHEDULER {
    class NewStaticScheduler : public Scheduler {
    public:
        NewStaticScheduler()= delete;
        NewStaticScheduler(std::vector<Task*> tasks, std::vector<Device*> device);
        void schedule() override;

    private:
    };
}

#endif //UI_NEWSTATICSCHEDULER_H
