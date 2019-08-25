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
        void enqueueTak(Task* task, Device *device, cl::CommandQueue commandQueue, cl::Kernel kernel, cl::Event &event);

    private:
        void getQueueTasksWithNoDependencies();
        void generateAllPrograms();

        std::vector<Task*> TasksToScheduleInStep;
        std::vector<Task*> TasksToReadInStep;
        std::vector<Task*> TasksToSchedule;
    };
}
