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
            cout<<"while(!TasksToSchedule.empty())"<<endl;
            getQueueTasksWithNoDependencies();
            std::vector<cl::Event> events;
            for (int i = 0; i < TasksToScheduleInStep.size(); i++) {
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
            cout<<"Before Wait for Events"<<endl;
            cl::Event::waitForEvents(events);
            cout<<"after Wait for Events"<<endl;
            for (int i = 0; i<TasksToReadInStep.size(); i++) {
                readDataFromTask(TasksToReadInStep.at(i), commandQueue);
                auto start = events.at(i).getProfilingInfo<CL_PROFILING_COMMAND_START>();
                auto end = events.at(i).getProfilingInfo<CL_PROFILING_COMMAND_END>();
                cout << start <<" - "<< end <<" = "<<end-start;
                TasksToReadInStep.at(i)->addElapsedTime(end-start);
            }
            cout<<"What the Actual Shit is going on"<<endl;
            TasksToReadInStep.clear();
        }
		deleteAllBuffers();
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
