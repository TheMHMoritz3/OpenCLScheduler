//
// Created by root on 25.08.19.
//

#pragma once

#include <queue>
#include "Scheduler.h"
#include "Task.h"
#include "Device.h"

namespace SCHEDULER {
    class ReadyFireScheduler : public Scheduler {
    public:
        ReadyFireScheduler()=delete;
        ReadyFireScheduler(std::vector<Task*> tasks, std::vector<Device*> device);

        void schedule() override;

    protected:
        void setRAMForCurrentTask(Task* task, Device *device, cl::Kernel kernel, cl::CommandQueue queue) override;

    private:
        void getQueueTasksWithNoDependencies();
        void generateAllPrograms();

        std::vector<Task*> TasksToScheduleInStep;
        std::vector<Task*> TasksToSchedule;
    };
}
