//
// Created by moritz on 17.06.19.
//

#ifndef EMBEDDEDSYSTEMSPROJECT_SCHEDULEMANAGER_H
#define EMBEDDEDSYSTEMSPROJECT_SCHEDULEMANAGER_H

#include <vector>

#include "SchedulerNamespace.h"
#include "Task.h"
#include "Scheduler.h"

namespace SCHEDULER {
    class ScheduleManager {
    public:
        ScheduleManager();
        void searchForDevices();
        void startSchedule();
        void setScheduleType(ScheduleType type);

    private:
        ScheduleType Type;
        std::vector<Task> Tasks;
        Scheduler* ActiveScheduler;
    };
}

#endif //EMBEDDEDSYSTEMSPROJECT_SCHEDULEMANAGER_H
