//
// Created by moritz on 17.06.19.
//

#include "ScheduleManager.h"
#include <iostream>
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
        for(cl::Device device : devices){
            Device newDevice(device_id, &device);
            Devices.push_back(newDevice);
        }
    }
}

void ScheduleManager::startSchedule() {

}

void ScheduleManager::setScheduleType(ScheduleType type) {

}

void ScheduleManager::addTask(std::string filePath, std::string kernelName) {
    Task task;
    task.setFile(filePath);
    task.setKernel(kernelName);
    Tasks.push_back(task);
}

bool ScheduleManager::isAddingTasksPossible() {
    return Devices.size()>0;
}

int ScheduleManager::getKernelCount() {
    return Tasks.size();
}
