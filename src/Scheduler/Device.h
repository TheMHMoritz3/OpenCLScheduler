//
// Created by moritz on 17.06.19.
//

#ifndef SCHEDULER_DEVICE_H
#define SCHEDULER_DEVICE_H

#include <string>
#include <CL/cl.hpp>
#include "Task.h"
#include "DevicePropertys.h"

namespace SCHEDULER {
	class Device {

    public:
        Device(int id, cl::Device oclDevice);
        ~Device();
        std::string getName() const;
        int scheduledTasks();
        int getWorkgroupSize();
        int getID();
        int getMaxComputeUnits();
        double getDeviceUsage();
        double getComputeUnitUsage(int ComputeUnit);
        void generateProgramm(Task* task);
        cl::Context getDeviceContext();
		cl::Device getOclDevice();

    private:
		DeviceProperties* Properties;
        cl::Device OclDevice;
        cl::Context OclContext;
        int Id;
    };
}

#endif //SCHEDULER_DEVICE_H
