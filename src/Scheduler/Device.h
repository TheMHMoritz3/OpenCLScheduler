//
// Created by moritz on 17.06.19.
//

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
