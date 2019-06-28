//
// Created by moritz on 17.06.19.
//

#include "ScheduleManager.h"
#include "StaticScheduler.h"
#include <iostream>
#include <fstream>
#include <CL/cl.hpp>

using namespace SCHEDULER;
using namespace std;

ScheduleManager::ScheduleManager() {

}

void ScheduleManager::searchForDevices() {
    vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    int device_id=0;

    for(cl::Platform platform : platforms){
        vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
        cout << "Init Devices"<<endl;
        for(cl::Device device : devices){
            Device newDevice(device_id, device);
            Devices.push_back(newDevice);
        }
    }
}

void ScheduleManager::startSchedule() {
    switch(Type){
        case ScheduleType::STATIC:
            ActiveScheduler = new StaticScheduler(Tasks, Devices);
            break;
        case ScheduleType::ASAPHC:
        case ScheduleType::LIST:
        default:
            break;
    }
    ActiveScheduler->schedule();
}

void ScheduleManager::setScheduleType(ScheduleType type) {
    Type=type;
}

Task* ScheduleManager::addTask(std::string filePath, std::string kernelName) {
    Task* task=new Task(getKernelCount());
    task->setProgramSources(convertSources(filePath));
    task->setKernel(kernelName);
    Tasks.push_back(task);
    return task;
}

bool ScheduleManager::isAddingTasksPossible() {
    return Devices.size()>0;
}

int ScheduleManager::getKernelCount() {
    return Tasks.size();
}

cl::Program::Sources* ScheduleManager::convertSources(std::string file) {
    cl::Program::Sources* sources = new cl::Program::Sources();

    ifstream sourceFile(file);
    string kernel_code(istreambuf_iterator<char>(sourceFile), (istreambuf_iterator<char>()));

    char* cstr = new char[kernel_code.length()+1];
    strcpy(cstr, kernel_code.c_str());

    sources->push_back({cstr,kernel_code.length()+1});
    return sources;
}

int ScheduleManager::getDeviceCount() {
    return Devices.size();
}
