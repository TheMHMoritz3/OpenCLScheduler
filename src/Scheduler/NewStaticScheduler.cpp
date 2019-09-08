//
// Created by moritz on 07.09.19.
//

#include "NewStaticScheduler.h"
#include <iostream>

using namespace SCHEDULER;

NewStaticScheduler::NewStaticScheduler(std::vector<Task *> tasks, std::vector<Device *> device) :
        Scheduler(tasks, device) {
    ErrorCode = 1;
    generateAllPrograms();
}


void NewStaticScheduler::schedule() {
    for (Device *device : Devices) {
        TasksToSchedule = Tasks;
        std::vector<cl::CommandQueue> commandQueues;

        while(!TasksToSchedule.empty()) {
            getQueueTasksWithNoDependencies();
            std::vector<cl::Event> events;
            for (int i = 0; i < TasksToScheduleInStep.size(); i++) {
                commandQueues.push_back(cl::CommandQueue(device->getDeviceContext(), device->getOclDevice()));
                cl::Event currentEvent;
                events.push_back(currentEvent);
                Task *task = TasksToScheduleInStep.at(i);
                cl::Kernel kernel = cl::Kernel(task->getProgramm(), task->getKernelName().c_str(), &ErrorCode);
                if (ErrorCode == CL_SUCCESS) {
                    SCHEDULER::Scheduler::setRAMForCurrentTask(task, device, kernel, commandQueues.at(i));
                    readConstantsFromTask(task, device, kernel, commandQueues.at(i));
                    setRAMBufferForOutput(task, device, kernel);
                    setKernelLoad(task, device, kernel);
                    enqueueTak(task, device, commandQueues.at(i), kernel,currentEvent);
                    TasksToReadInStep.push_back(task);
                } else
                    std::cout << "First Step Kernel Creation Resolved Error: " << ErrorCode << std::endl;
            }
            cl::Event::waitForEvents(events);
            for (int i = 0; i<TasksToReadInStep.size(); i++) {
                readDataFromTask(TasksToReadInStep.at(i), commandQueues.at(i));
            }
            TasksToReadInStep.clear();
        }
        deleteAllBuffers();
    }
}

void NewStaticScheduler::getQueueTasksWithNoDependencies() {
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
//    std::cout << "Tasks to Schedule: " << TasksToSchedule.size() << endl;
}

void NewStaticScheduler::generateAllPrograms() {
    for (Device *device : Devices)
        for (Task *task : Tasks)
            device->generateProgramm(task);
}

void NewStaticScheduler::enqueueTak(Task *task, Device *device, cl::CommandQueue commandQueue, cl::Kernel kernel, cl::Event &event) {
    ErrorCode = commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(CoreCount), cl::NDRange(CoreCount),NULL,&event);
//    std::cout << "Enqueue Task: " << ErrorCode << std::endl;
}