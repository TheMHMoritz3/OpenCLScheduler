//
// Created by moritz on 17.06.19.
//

#include "StaticScheduler.h"

using namespace SCHEDULER;

StaticScheduler::StaticScheduler(std::vector<Task> tasks, std::vector<Device> device) : Scheduler(tasks, device) {
	ErrorCode = 1;
	generateCommandQueues();
}


void StaticScheduler::schedule() {

}

void StaticScheduler::generateCommandQueues()
{
	for (Device device : Devices) {
		cl::CommandQueue commandQueue(device.getDeviceContext(), device.getOclDevice(), ErrorCode);
		CommandQueues.push_back(commandQueue);
	}
}
