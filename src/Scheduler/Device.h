//
// Created by moritz on 17.06.19.
//

#ifndef CAN_DEVICE_H
#define CAN_DEVICE_H

#include <string>
#include <CL/cl.hpp>
#include "Task.h"

namespace SCHEDULER {
#ifdef _WINDOWS
    class __declspec(dllexport) Device {
#endif
#ifndef _WINDOWS
	class Device {
#endif // !_WINDOWS

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
        void schedule(Task tak, int computeUnit);
        void generateProgramm(Task task);
        cl::Context getDeviceContext();
        Device operator=(Device other);
    private:
        cl::Device const OclDevice;
        cl::Context OclContext;
        int Id;
    };
}

#endif //CAN_DEVICE_H
