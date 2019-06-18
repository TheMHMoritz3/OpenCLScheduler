//
// Created by moritz on 17.06.19.
//

#ifndef CAN_DEVICE_H
#define CAN_DEVICE_H

#include <string>
#include <CL/cl.hpp>
#include "Task.h"

namespace SCHEDULER {
    class Device {
    public:
        Device(int id, cl::Device* oclDevice);
        ~Device();
        std::string getName();
        int scheduledTasks();
        int getWorkgroupSize();
        int getID();
        int getMaxComputeUnits();
        double getDeviceUsage();
        double getComputeUnitUsage(int ComputeUnit);
        void schedule(Task tak, int computeUnit);

    private:
//        cl::Platform* const Platform;
        cl::Device* const OclDevice;
        int Id;
    };
}

#endif //CAN_DEVICE_H
