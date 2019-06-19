//
// Created by moritz on 17.06.19.
//

#ifndef EMBEDDEDSYSTEMSPROJECT_SCHEDULEMANAGER_H
#define EMBEDDEDSYSTEMSPROJECT_SCHEDULEMANAGER_H

#include <vector>
#include <string>
#include <CL/cl.hpp>

#include "SchedulerNamespace.h"
#include "Task.h"
#include "Scheduler.h"
#include "Device.h"

namespace SCHEDULER {
    class ScheduleManager {
    public:
        ScheduleManager();
        void searchForDevices();
        void startSchedule();
        void setScheduleType(ScheduleType type);
        void addTask(std::string filePath, std::string kernelName);
        bool isAddingTasksPossible();
        int getKernelCount();

    private:
        cl::Program::Sources convertSources(std::string file);

        ScheduleType Type;
        std::vector<Task> Tasks;
        std::vector<Device> Devices;
        Scheduler* ActiveScheduler;
    };
}

#endif //EMBEDDEDSYSTEMSPROJECT_SCHEDULEMANAGER_H
