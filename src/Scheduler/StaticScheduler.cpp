//
// Created by moritz on 17.06.19.
//

#include "StaticScheduler.h"
#include <CL/cl.hpp>

using namespace SCHEDULER;

StaticScheduler::StaticScheduler(std::vector<Task> tasks, std::vector<Device> device) : Scheduler(tasks, device) {
	ErrorCode = 1;
}


void StaticScheduler::schedule() {
	for (Device device : Devices) {
		cl::CommandQueue commandQueue(device.getDeviceContext(), device.getOclDevice(), ErrorCode);
		CommandQueues.push_back(commandQueue);
		for (Task task : Tasks) {
			cl::Kernel kernel = cl::Kernel(task.getProgramm(), task.getKernelName().c_str(), &ErrorCode);
			setRAMForCurrentTask(task, device, kernel, commandQueue);
			setRAMBufferForOutput(task, device, kernel);
			setKernelLoad(task, device, kernel);
			readDataFromTask(task, commandQueue);
		}
	}
}

