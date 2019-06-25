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
		cl::CommandQueue commandQueue(device.getDeviceContext(), device.getOclDevice(), ErrorCode);
		cout << "Error Code: "<<ErrorCode<<endl;
		CommandQueues.push_back(commandQueue);
		for (Task* task : Tasks) {

			//cout << "Generate Programm for: " << device.getName() << endl;

			device.generateProgramm(task);

			//cout << "Kernel Name: " << task->getKernelName() << endl;
			cl::Kernel kernel = cl::Kernel(task->getProgramm(), task->getKernelName().c_str(),  &ErrorCode);
			//cout << "Error Code for generating Kernel: "<<ErrorCode <<" Kernel Name: "<<task->getKernelName()<<"-"<<kernel.getInfo<CL_KERNEL_FUNCTION_NAME>()<<endl;
			if (ErrorCode == CL_SUCCESS) {
				setRAMForCurrentTask(task, device, kernel, commandQueue);
				setRAMBufferForOutput(task, device, kernel);
				setKernelLoad(task, device, kernel);
				enqueueTak(task, device, commandQueue, kernel);
				readDataFromTask(task, commandQueue);
			}
		}
	}
}

