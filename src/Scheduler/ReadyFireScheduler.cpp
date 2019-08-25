//
// Created by root on 25.08.19.
//

#include "ReadyFireScheduler.h"
#include <iostream>

using namespace std;
using namespace SCHEDULER;

ReadyFireScheduler::ReadyFireScheduler(std::vector<Task *> tasks, std::vector<Device *> device)
        : Scheduler(tasks, device) {
    generateAllPrograms();
}

void ReadyFireScheduler::schedule() {
    for (Device *device : Devices) {
        TasksToSchedule = Tasks;
        cl::CommandQueue commandQueue;

//        if (device->getProperties()->getOutOfOrderExecution())
//            commandQueue = cl::CommandQueue(device->getDeviceContext(), device->getOclDevice(), CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);
//        else
        commandQueue = cl::CommandQueue(device->getDeviceContext(), device->getOclDevice());
        CommandQueues.push_back(commandQueue);
        getQueueTasksWithNoDependencies();

        for (int i = 0; i < TasksToScheduleInStep.size(); i++) {
            Task *task = TasksToScheduleInStep.at(i);
            cl::Kernel kernel = cl::Kernel(task->getProgramm(), task->getKernelName().c_str(), &ErrorCode);
            if (ErrorCode == CL_SUCCESS) {
                SCHEDULER::Scheduler::setRAMForCurrentTask(task, device, kernel, commandQueue);
                readConstantsFromTask(task, device, kernel, commandQueue);
                setRAMBufferForOutput(task, device, kernel);
                setKernelLoad(task, device, kernel);
                enqueueTak(task, device, commandQueue, kernel);
            } else
                cout << "First Step Kernel Creation Resolved Error: " << ErrorCode << endl;
        }
        for (Task *task : TasksToSchedule) {
            cl::Kernel kernel = cl::Kernel(task->getProgramm(), task->getKernelName().c_str(), &ErrorCode);
            if (ErrorCode == CL_SUCCESS) {
                setRAMForCurrentTask(task, device, kernel, commandQueue);
                readConstantsFromTask(task, device, kernel, commandQueue);
                setRAMBufferForOutput(task, device, kernel);
                setKernelLoad(task, device, kernel);
                enqueueTak(task, device, commandQueue, kernel);
            } else
                cout << "Second Step Kernel Creation Resolved Error: " << ErrorCode << endl;
        }
        commandQueue.finish();
        for (Task *task : Tasks) {
            readDataFromTask(task, commandQueue);
        }
    }
}

void ReadyFireScheduler::setRAMForCurrentTask(Task *task, Device *device, cl::Kernel kernel, cl::CommandQueue queue) {
    int count = 0;
    for (Task *depTask : task->getDependantTasks()) {
        ErrorCode = queue.enqueueWriteBuffer(*task->readBuffer(),CL_TRUE,count,task->readBuffer()->getInfo<CL_MEM_SIZE>(),task->readBuffer()->getInfo<CL_MEM_HOST_PTR>());
        ErrorCode = kernel.setArg(count, *task->readBuffer());
        count++;

        std::cout << "Ready Fire Scheduler Set Ram for Current Task: " << ErrorCode << std::endl;
        std::cout << "Count " << count << std::endl;
    }
}

void ReadyFireScheduler::getQueueTasksWithNoDependencies() {
    bool TasksAreRemoved = true;
    while (TasksAreRemoved) {
        TasksAreRemoved = false;
        for (int i = 0; i < TasksToSchedule.size(); i++) {
            if (TasksToSchedule.at(i)->dependenciesAreCalculated()) {
                TasksToScheduleInStep.push_back(TasksToSchedule.at(i));
                TasksToSchedule.erase(TasksToSchedule.begin() + i);
                TasksAreRemoved = true;
            }
            i++;
        }
    }
    std::cout << "Tasks to Schedule in Step: " << TasksToScheduleInStep.size() << endl;
}

void ReadyFireScheduler::generateAllPrograms() {
    for (Device *device : Devices)
        for (Task *task : Tasks)
            device->generateProgramm(task);
}
