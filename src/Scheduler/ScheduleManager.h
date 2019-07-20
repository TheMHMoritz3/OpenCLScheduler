//
// Created by moritz on 17.06.19.
//

#ifndef EMBEDDEDSYSTEMSPROJECT_SCHEDULEMANAGER_H
#define EMBEDDEDSYSTEMSPROJECT_SCHEDULEMANAGER_H

#include <vector>
#include <string>
#include <functional>
#include <CL/cl.hpp>

#include "SchedulerNamespace.h"
#include "Task.h"
#include "Scheduler.h"
#include "Device.h"
#include "scheduler_global.h"

namespace SCHEDULER {
	class SCHEDULER_EXPORT ScheduleManager {
    public:
		ScheduleManager();
		void searchForDevices();
        void startSchedule();
        void setScheduleType(ScheduleType type);

		Task* addTask(std::string filePath, std::string kernelName);
        bool isAddingTasksPossible();
        int getKernelCount();
        int getDeviceCount();
		DeviceProperties* getDeviceProperties(int i) const;

    private:
		cl::Program::Sources* convertSources(std::string file);

        ScheduleType Type;
        std::vector<Task*> Tasks;
        std::vector<Device*> Devices;
        Scheduler* ActiveScheduler;
    };
}

#endif //EMBEDDEDSYSTEMSPROJECT_SCHEDULEMANAGER_H
