//
// Created by moritz on 07.09.19.
//

#include "NewStaticScheduler.h"
#include <iostream>

using namespace SCHEDULER;

NewStaticScheduler::NewStaticScheduler(std::vector<Task *> tasks, std::vector<Device *> device) :
        Scheduler(tasks, device) {
    ErrorCode = 1;
}


void NewStaticScheduler::schedule() {
    for (Device* device : Devices) {
//		cout << "Device Name: " << device->getName()<<endl;
        cl::CommandQueue commandQueue(device->getDeviceContext(), device->getOclDevice(), CL_QUEUE_PROFILING_ENABLE);
        CommandQueues.push_back(commandQueue);
        for (Task* task : Tasks) {
//		    cout << "Task Name: "<<task->getKernelName()<<endl;
            device->generateProgramm(task);

            cl::Kernel kernel = cl::Kernel(task->getProgramm(), task->getKernelName().c_str(),  &ErrorCode);
            if (ErrorCode == CL_SUCCESS) {
                setRAMForCurrentTask(task, device, kernel, commandQueue);
                readConstantsFromTask(task, device, kernel, commandQueue);
                setRAMBufferForOutput(task, device, kernel);
                setKernelLoad(task, device, kernel);
                commandQueue.finish();
                enqueueTak(task, device, commandQueue, kernel);
                commandQueue.finish();
                readDataFromTask(task, commandQueue);
            }
            else
                std::cout << "Kernel Creation Resolved Error: " << ErrorCode<<std::endl;
        }
        deleteAllBuffers();
    }
}