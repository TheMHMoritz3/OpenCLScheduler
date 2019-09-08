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

#endif //UI_NEWSTATICSCHEDULER_H
