//
// Created by moritz on 17.06.19.
//

#ifndef EMBEDDEDSYSTEMSPROJECT_SCHEDULER_H
#define EMBEDDEDSYSTEMSPROJECT_SCHEDULER_H

#include <vector>
#include <CL/cl.hpp>
#include "Task.h"
#include "Device.h"
#include "SchedulerNamespace.h"

namespace SCHEDULER {
#ifdef _WINDOWS
	class __declspec(dllexport) Scheduler {
#endif
#ifndef _WINDOWS
	class Scheduler {
#endif // !_WINDOWS
    public:
        Scheduler() = delete;
        Scheduler(std::vector<Task> tasks, std::vector<Device> devices);
        virtual void schedule()=0;

    protected:
        std::vector<Task> Tasks;
        std::vector<Device> Devices;
        void setRAMForCurrentTask(Task task, Device device, cl::Kernel kernel, cl::CommandQueue queue);
		void setRAMBufferForOutput(Task task, Device device, cl::Kernel kernel);
		void setKernelLoad(Task task, cl::Kernel kernel);
    private:
        cl::Buffer generateBufferForUINT(void* data,cl::Context context ,cl::CommandQueue queue, int count);
        cl::Buffer generateBufferForINT(void* data,cl::Context context ,cl::CommandQueue queue, int count);
        cl::Buffer generateBufferForCHAR(void* data,cl::Context context ,cl::CommandQueue queue, int count);
        cl::Buffer generateBufferForDOUBLE(void* data,cl::Context context ,cl::CommandQueue queue, int count);
        cl::Buffer generateBufferForFLOAT(void* data,cl::Context context ,cl::CommandQueue queue, int count);
    };
}


#endif //EMBEDDEDSYSTEMSPROJECT_SCHEDULER_H
