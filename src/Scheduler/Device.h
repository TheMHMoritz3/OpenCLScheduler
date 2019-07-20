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

#ifndef SCHEDULER_DEVICE_H
#define SCHEDULER_DEVICE_H

#include <string>
#include <CL/cl.hpp>
#include "Task.h"
#include "scheduler_global.h"

namespace SCHEDULER {
	class SCHEDULER_EXPORT Device {

    public:
        Device(int id, cl::Device oclDevice);
        ~Device();
        std::string getName();
        int scheduledTasks();
        int getWorkgroupSize();
        int getID();
        int getMaxComputeUnits();
        double getDeviceUsage();
        double getComputeUnitUsage(int ComputeUnit);
        void generateProgramm(Task* task);
        cl::Context getDeviceContext();
		cl::Device getOclDevice();
        Device operator=(Device other);
    private:
        cl::Device OclDevice;
        cl::Context OclContext;
        int Id;
    };
}

#endif //SCHEDULER_DEVICE_H
