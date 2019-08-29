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

#ifndef EMBEDDEDSYSTEMSPROJECT_SCHEDULER_H
#define EMBEDDEDSYSTEMSPROJECT_SCHEDULER_H

#include <vector>
#include <CL/cl.hpp>
#include "Task.h"
#include "Device.h"
#include "SchedulerNamespace.h"

namespace SCHEDULER {
	class Scheduler {
    public:
        Scheduler() = delete;
        Scheduler(std::vector<Task*> tasks, std::vector<Device*> devices);
        virtual void schedule()=0;

        void setCoreCount(int cores);
    protected:
        std::vector<Task*> Tasks;
        std::vector<Device*> Devices;
		std::vector<cl::CommandQueue> CommandQueues;
		cl_int ErrorCode;
        int CoreCount;
        virtual void setRAMForCurrentTask(Task* task, Device *device, cl::Kernel kernel, cl::CommandQueue queue);
		virtual void readConstantsFromTask(Task* task, Device* device, cl::Kernel kernel, cl::CommandQueue commandQueue);
		virtual void setRAMBufferForOutput(Task* task, Device *device, cl::Kernel kernel);
		virtual void setKernelLoad(Task* task, Device *device, cl::Kernel kernel);
		virtual void enqueueTak(Task* task, Device *device, cl::CommandQueue commandQueue, cl::Kernel kernel);
		virtual void readDataFromTask(Task* task, cl::CommandQueue commandQueue);
    private:
        cl::Buffer *generateBufferForUINT(std::vector<void*>,cl::Context context ,cl::CommandQueue queue, int count);
        cl::Buffer *generateBufferForINT(std::vector<void*>,cl::Context context ,cl::CommandQueue queue, int count);
        cl::Buffer *generateBufferForCHAR(std::vector<void*>,cl::Context context ,cl::CommandQueue queue, int count);
        cl::Buffer *generateBufferForDOUBLE(std::vector<void*>,cl::Context context ,cl::CommandQueue queue, int count);
        cl::Buffer *generateBufferForFLOAT(std::vector<void*>,cl::Context context ,cl::CommandQueue queue, int count);

		cl::Buffer* generateBufferForUINTConstant(void* data, cl::Context context, cl::CommandQueue queue, int count, int load);
		cl::Buffer* generateBufferForINTConstant(void* data, cl::Context context, cl::CommandQueue queue, int count, int load);
		cl::Buffer* generateBufferForFLOATConstant(void* data, cl::Context context, cl::CommandQueue queue, int count, int load);
		cl::Buffer* generateBufferForDOUBLEConstant(void* data, cl::Context context, cl::CommandQueue queue, int count, int load);
		cl::Buffer* generateBufferForCHARConstant(void* data, cl::Context context, cl::CommandQueue queue, int count, int load);

		std::vector<void*> readDataFromBufferForUINT(Task* task, cl::CommandQueue queue, int count);
		std::vector<void*> readDataFromBufferForINT(Task* task, cl::CommandQueue queue, int count);
		std::vector<void*> readDataFromBufferForCHAR(Task* task, cl::CommandQueue queue, int count);
		std::vector<void*> readDataFromBufferForDOUBLE(Task* task, cl::CommandQueue queue, int count);
		std::vector<void*> readDataFromBufferForFLOAT(Task* task, cl::CommandQueue queue, int count);
    };
}


#endif //EMBEDDEDSYSTEMSPROJECT_SCHEDULER_H