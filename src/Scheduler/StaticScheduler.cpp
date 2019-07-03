//
// Created by moritz on 17.06.19.
//

#include "StaticScheduler.h"
#include <iostream>
#include <string>
#include <cstring>
#include <CL/cl.hpp>
#include <fstream>

using namespace SCHEDULER;
using namespace std;

StaticScheduler::StaticScheduler(std::vector<Task*> tasks, std::vector<Device> device) : Scheduler(tasks, device) {
	ErrorCode = 1;
}


void StaticScheduler::schedule() {
	for (Device device : Devices) {
		cout << "Device Name: " << device.getName()<<endl;
		cl::CommandQueue commandQueue(device.getDeviceContext(), device.getOclDevice());
		CommandQueues.push_back(commandQueue);
		for (Task* task : Tasks) {
			device.generateProgramm(task);

			cl::Kernel kernel = cl::Kernel(task->getProgramm(), task->getKernelName().c_str(),  &ErrorCode);
            if (ErrorCode == CL_SUCCESS) {
				setRAMForCurrentTask(task, device, kernel, commandQueue);
				setRAMBufferForOutput(task, device, kernel);
				setKernelLoad(task, device, kernel);
				enqueueTak(task, device, commandQueue, kernel);
				readDataFromTask(task, commandQueue);
			}
			else
				cout << "Kernel Creation Resolved Error: " << ErrorCode<<endl;
		}
	}
}

