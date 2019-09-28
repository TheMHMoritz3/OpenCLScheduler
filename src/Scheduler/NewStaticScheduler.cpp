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
    scheduleTasks(Tasks);
}


void NewStaticScheduler::schedule() {
    for (Device *device : Devices) {
        cl::CommandQueue commandQueue;
        commandQueue = cl::CommandQueue(device->getDeviceContext(), device->getOclDevice());
        for(int j=0; j<(int)TasksToSchedule.size();j++){
            std::vector<Task*> tasks = TasksToSchedule.at(j);
            for(int i = 0; i<(int)tasks.size();i++){
                Task* task = tasks.at(i);
                cl::Kernel kernel = cl::Kernel(task->getProgramm(), task->getKernelName().c_str(), &ErrorCode);
                if (ErrorCode == CL_SUCCESS) {
                    setRAMForCurrentTask(task, device, kernel, commandQueue);
                    readConstantsFromTask(task, device, kernel, commandQueue);
                    setRAMBufferForOutput(task, device, kernel);
                    setKernelLoad(task, device, kernel);
                    enqueueTak(task, device, commandQueue, kernel);
                }
                else
                    std::cout << "Kernel Creation Resolved Error: " << ErrorCode << std::endl;
            }
            commandQueue.finish();
            for(int i = 0; i<(int)tasks.size();i++){
                Task* task = tasks.at(i);
                readDataFromTask(task, commandQueue);
            }
        }
    }
}


void NewStaticScheduler::generateAllPrograms() {
    for (Device *device : Devices)
        for (Task *task : Tasks)
            device->generateProgramm(task);
}

bool NewStaticScheduler::areTaskDependenciesScheduled(Task* task) {
    for(Task* dtask : task->getDependantTasks()){
        if(!dtask->isScheduled())
            return false;
    }
    return true;
}

void NewStaticScheduler::scheduleTasks(std::vector<Task *> tasks) {
    bool AnythingScheduled;
    do{
        AnythingScheduled=false;
        std::vector<Task*> scheduledTasks;
        for(Task *task : tasks)
        {
            if((areTaskDependenciesScheduled(task))&&(!task->isScheduled())){
                scheduledTasks.push_back(task);
                AnythingScheduled=true;
            }
        }
        for(Task* task : scheduledTasks) {
            task->setScheduled(true);
        }
        if(AnythingScheduled)
            TasksToSchedule.emplace_back(scheduledTasks);

    }while(AnythingScheduled);
}
