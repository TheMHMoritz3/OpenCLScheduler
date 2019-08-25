//
// Created by root on 25.08.19.
//

#pragma once

#include "Scheduler.h"
#include "Task.h"
#include "Device.h"

namespace SCHEDULER {
    class ReadyFireScheduler : public Scheduler {
    public:
        ReadyFireScheduler()=delete;
        ReadyFireScheduler(std::vector<Task*> tasks, std::vector<Device*> device);

        void schedule() override;
    };
}
