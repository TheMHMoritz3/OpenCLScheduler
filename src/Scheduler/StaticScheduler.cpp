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

            cl::Program::Sources sources;

            ifstream sourceFile(task->getFilePath());
            string kernel_code(istreambuf_iterator<char>(sourceFile), (istreambuf_iterator<char>()));

            char cstr[kernel_code.length()+1];
            strcpy(cstr, kernel_code.c_str());

            sources.push_back({cstr,kernel_code.length()+1});
//            cout<<"ScheduleManager::convertSources"<<sources.at(0).first<<endl;

			cout << "Generate Programm for: " << device.getName() << endl;

            cl::Program program(device.getDeviceContext(), sources);
            cout <<"Sources"<< task->getSources()->at(0).first<<endl;
            if(program.build({device.getOclDevice()})!=CL_SUCCESS)
                cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device.getOclDevice()) << "\n";
            else
                cout << "Success" <<endl;
            task->setProgam(program);

			cout << "Kernel Name: " << task->getKernelName() << endl;
			cl::Kernel kernel = cl::Kernel(task->getProgramm(), task->getKernelName().c_str(),  &ErrorCode);
			cout << "Error Code for generating Kernel: "<<ErrorCode <<" Kernel Name: "<<task->getKernelName()<<"-"<<kernel.getInfo<CL_KERNEL_FUNCTION_NAME>()<<endl;
			if (ErrorCode == CL_SUCCESS) {
				setRAMForCurrentTask(task, device, kernel, commandQueue);
				setRAMBufferForOutput(task, device, kernel);
				setKernelLoad(task, device, kernel);
				readDataFromTask(task, commandQueue);
			}
		}
	}
}

