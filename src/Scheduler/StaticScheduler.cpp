//
// Created by moritz on 17.06.19.
//

#include "StaticScheduler.h"
#include <iostream>
#include <CL/cl.hpp>

using namespace SCHEDULER;
using namespace std;

StaticScheduler::StaticScheduler(std::vector<Task*> tasks, std::vector<Device> device) : Scheduler(tasks, device) {
	ErrorCode = 1;
}


void StaticScheduler::schedule() {
	for (Device device : Devices) {
		cl::CommandQueue commandQueue(device.getDeviceContext(), device.getOclDevice(), ErrorCode);
		CommandQueues.push_back(commandQueue);
		for (Task* task : Tasks) {
			cout << "Generate Programm for: " << device.getName() << endl;
			device.generateProgramm(task);
			char* cstr = new char[task->getKernelName().length() + 1];
			strcpy(cstr, task->getKernelName().c_str());
			cout << "Kernel Name: " << cstr << endl;
			cl::Kernel kernel = cl::Kernel(task->getProgramm(), cstr, &ErrorCode);
			cout << "Error Code for generating Kernel: "<<ErrorCode <<" Kernel Name: "<<task->getKernelName()<<"-"<<kernel.getInfo<CL_KERNEL_FUNCTION_NAME>()<<endl;
			if (ErrorCode == 0) {
				setRAMForCurrentTask(task, device, kernel, commandQueue);
				setRAMBufferForOutput(task, device, kernel);
				setKernelLoad(task, device, kernel);
				readDataFromTask(task, commandQueue);
			}
		}
	}
}

