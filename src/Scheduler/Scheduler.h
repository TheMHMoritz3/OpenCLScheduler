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
		std::vector<cl::CommandQueue> CommandQueues;
		cl_int ErrorCode;
        void setRAMForCurrentTask(Task task, Device device, cl::Kernel kernel, cl::CommandQueue queue);
		void setRAMBufferForOutput(Task task, Device device, cl::Kernel kernel);
		void setKernelLoad(Task task, Device device, cl::Kernel kernel);
		void enqueueTak(Task task, Device device, cl::CommandQueue commandQueue, cl::Kernel kernel);
		void readDataFromTask(Task task, cl::CommandQueue commandQueue);
    private:
        cl::Buffer generateBufferForUINT(void* data,cl::Context context ,cl::CommandQueue queue, int count);
        cl::Buffer generateBufferForINT(void* data,cl::Context context ,cl::CommandQueue queue, int count);
        cl::Buffer generateBufferForCHAR(void* data,cl::Context context ,cl::CommandQueue queue, int count);
        cl::Buffer generateBufferForDOUBLE(void* data,cl::Context context ,cl::CommandQueue queue, int count);
        cl::Buffer generateBufferForFLOAT(void* data,cl::Context context ,cl::CommandQueue queue, int count);
		void* readDataFromBufferForUINT(Task task, cl::CommandQueue queue, int count);
		void* readDataFromBufferForINT(Task task, cl::CommandQueue queue, int count);
		void* readDataFromBufferForCHAR(Task task, cl::CommandQueue queue, int count);
		void* readDataFromBufferForDOUBLE(Task task, cl::CommandQueue queue, int count);
		void* readDataFromBufferForFLOAT(Task task, cl::CommandQueue queue, int count);
    };
}


#endif //EMBEDDEDSYSTEMSPROJECT_SCHEDULER_H
