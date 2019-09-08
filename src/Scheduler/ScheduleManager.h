/*
Embedded Systems Project 2019
Copyright (C) 2019  Moritz Herzog, Philip Lersch

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
		void startMultiDeviceScheduling();
		void startSingleDeviceScheduling();
        void setScheduleType(ScheduleType type);

		Task* addTask(std::string filePath, std::string kernelName);
        bool isAddingTasksPossible();
        int getKernelCount();
        int getDeviceCount();
		DeviceProperties* getDeviceProperties(int i) const;
		void setActiveDevice(int id);

		int getLastScheduleTime() const;

    private:
		void startSchedule(std::vector<Task*>tasks, Device* device);
		cl::Program::Sources* convertSources(std::string file);

        ScheduleType Type;
        std::vector<Task*> Tasks;
        std::vector<Device*> Devices;
        Scheduler* ActiveScheduler;
        Device* ActiveDevice;
        int LastScheduleTime;
    };
}

#endif //EMBEDDEDSYSTEMSPROJECT_SCHEDULEMANAGER_H
