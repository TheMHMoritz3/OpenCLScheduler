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

        if (device->getProperties()->getOutOfOrderExecution())
            commandQueue = cl::CommandQueue(device->getDeviceContext(), device->getOclDevice(), CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE);
        else
        commandQueue = cl::CommandQueue(device->getDeviceContext(), device->getOclDevice(),CL_QUEUE_PROFILING_ENABLE);
        CommandQueues.push_back(commandQueue);
        while(!TasksToSchedule.empty()) {
            getQueueTasksWithNoDependencies();
            std::vector<cl::Event> events;
            for (int i = 0; i < (int)TasksToScheduleInStep.size(); i++) {
                cl::Event currentEvent;
                events.push_back(currentEvent);
                Task *task = TasksToScheduleInStep.at(i);
                cl::Kernel kernel = cl::Kernel(task->getProgramm(), task->getKernelName().c_str(), &ErrorCode);
                if (ErrorCode == CL_SUCCESS) {
                    SCHEDULER::Scheduler::setRAMForCurrentTask(task, device, kernel, commandQueue);
                    readConstantsFromTask(task, device, kernel, commandQueue);
                    setRAMBufferForOutput(task, device, kernel);
                    setKernelLoad(task, device, kernel);
                    enqueueTak(task, device, commandQueue, kernel,currentEvent);
                    TasksToReadInStep.push_back(task);
                } else
                    cout << "First Step Kernel Creation Resolved Error: " << ErrorCode << endl;
            }
            cl::Event::waitForEvents(events);
            for (int i = 0; i<(int)TasksToReadInStep.size(); i++) {
                readDataFromTask(TasksToReadInStep.at(i), commandQueue);
            }
            TasksToReadInStep.clear();
        }
		deleteAllBuffers();
    }
}

void ReadyFireScheduler::getQueueTasksWithNoDependencies() {
    bool TasksAreRemoved = true;
    while (TasksAreRemoved) {
        TasksAreRemoved = false;
        for (int i = 0; i < (int)TasksToSchedule.size(); i++) {
            if (TasksToSchedule.at(i)->dependenciesAreCalculated()) {
                TasksToScheduleInStep.push_back(TasksToSchedule.at(i));
                TasksToSchedule.erase(TasksToSchedule.begin() + i);
                TasksAreRemoved = true;
            }
            i++;
        }
    }
//    std::cout << "Tasks to Schedule: " << TasksToSchedule.size() << endl;
}

void ReadyFireScheduler::generateAllPrograms() {
    for (Device *device : Devices)
        for (Task *task : Tasks)
            device->generateProgramm(task);
}

void ReadyFireScheduler::enqueueTak(Task *task, Device *device, cl::CommandQueue commandQueue, cl::Kernel kernel, cl::Event &event) {
    ErrorCode = commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(CoreCount), cl::NDRange(CoreCount),NULL,&event);
//    std::cout << "Enqueue Task: " << ErrorCode << std::endl;
}
