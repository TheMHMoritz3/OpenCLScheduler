//
// Created by moritz on 07.09.19.
//

#ifndef UI_NEWSTATICSCHEDULER_H
#define UI_NEWSTATICSCHEDULER_H

#include "Scheduler.h"
#include "Device.h"
#include <vector>
#include <queue>
#include "Task.h"

namespace SCHEDULER {
    class NewStaticScheduler : public Scheduler {
    public:
        NewStaticScheduler()= delete;
        NewStaticScheduler(std::vector<Task*> tasks, std::vector<Device*> device);
        void schedule() override;

    private:
        void generateAllPrograms();
        bool areTaskDependenciesScheduled(Task* task);
        void scheduleTasks(std::vector<Task*> tasks);

        std::queue<std::vector<Task*>> TasksToSchedule;
    };
}

#endif //UI_NEWSTATICSCHEDULER_H
