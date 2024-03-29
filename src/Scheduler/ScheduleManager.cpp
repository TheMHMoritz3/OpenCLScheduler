/*
Embedded Systems Project 2019
Copyright (C) 2019  Moritz Herzog, Philip Lersch

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScheduleManager.h"
#include "StaticScheduler.h"
#include "ASAP.h"
#include "ReadyFireScheduler.h"
#include "NewStaticScheduler.h"
#include <iostream>
#include <fstream>
#include <CL/cl.hpp>
#include <chrono>

using namespace SCHEDULER;
using namespace std;

ScheduleManager::ScheduleManager() {
    LastScheduleTime = 0;
}

void ScheduleManager::searchForDevices() {
    vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    int device_id = 0;

    for (cl::Platform platform : platforms) {
        vector<cl::Device> devices;
        platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
        for (cl::Device device : devices) {
            Device *newDevice = new Device(device_id, device);
            Devices.push_back(newDevice);
            device_id++;
        }
    }
}

void ScheduleManager::startSchedule() {
    switch (Type) {
        case ScheduleType::SERIAL:
            ActiveScheduler = new StaticScheduler(Tasks, Devices);
            break;
        case ScheduleType::STATIC:
            ActiveScheduler = new NewStaticScheduler(Tasks, Devices);
            break;
        case ScheduleType::ASAPHC:
            ActiveScheduler = new ASAP(Tasks, Devices);
            break;
        case ScheduleType::READY_FIRE_SCHEDULER:
            ActiveScheduler = new ReadyFireScheduler(Tasks, Devices);
            break;
        default:
            break;
    }
    auto start = std::chrono::steady_clock::now();
    ActiveScheduler->schedule();
    auto end = std::chrono::steady_clock::now();
    LastScheduleTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

void ScheduleManager::startMultiDeviceScheduling() {
    for (Device *device : Devices) {
        DeviceProperties *props = device->getProperties();
        Type = props->getSchedule();
        startSchedule(props->getTasksToSchedule(), device);
    }
}

void ScheduleManager::startSchedule(std::vector<Task *> tasks, Device *device) {
    std::vector<Device *> devices;
    devices.push_back(device);
    switch (Type) {
        case ScheduleType::SERIAL:
            ActiveScheduler = new StaticScheduler(tasks, devices);
            break;
        case ScheduleType::STATIC:
        case ScheduleType::ASAP_POCL:
            ActiveScheduler = new NewStaticScheduler(tasks, devices);
            break;

        case ScheduleType::ASAPHC:
        case ScheduleType::STATIC_POCL:
            ActiveScheduler = new ASAP(tasks, devices);
            break;
        case ScheduleType::READY_FIRE_SCHEDULER:
            ActiveScheduler = new ReadyFireScheduler(tasks, devices);
            break;
        default:
            break;
    }
    if (device->getProperties()->getCoureCount() > 1) {
        ActiveScheduler->setCoreCount(device->getProperties()->getCoureCount());
    }
    auto start = std::chrono::steady_clock::now();
    ActiveScheduler->schedule();
    auto end = std::chrono::steady_clock::now();
    LastScheduleTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

void ScheduleManager::setScheduleType(ScheduleType type) {
    Type = type;
}

Task *ScheduleManager::addTask(std::string filePath, std::string kernelName) {
    Task *task = new Task(getKernelCount());
    task->setProgramSources(convertSources(filePath));
    task->setKernel(kernelName);
    task->setPath(filePath);
    Tasks.push_back(task);
    return task;
}

bool ScheduleManager::isAddingTasksPossible() {
    return Devices.size() > 0;
}

int ScheduleManager::getKernelCount() {
    return Tasks.size();
}

cl::Program::Sources *ScheduleManager::convertSources(std::string file) {
    cl::Program::Sources *sources = new cl::Program::Sources();

    ifstream sourceFile(file);
    string kernel_code(istreambuf_iterator<char>(sourceFile), (istreambuf_iterator<char>()));

    char *cstr = new char[kernel_code.length() + 1];
    strcpy(cstr, kernel_code.c_str());

    sources->push_back({cstr, kernel_code.length() + 1});
    return sources;
}

int ScheduleManager::getDeviceCount() {
    return Devices.size();
}

DeviceProperties *ScheduleManager::getDeviceProperties(int i) const {
    return Devices.at(i)->getProperties();
}

void ScheduleManager::setActiveDevice(int id) {
    ActiveDevice = Devices.at(id);
}

void ScheduleManager::startSingleDeviceScheduling() {
    Type = ActiveDevice->getProperties()->getSchedule();
    startSchedule(ActiveDevice->getProperties()->getTasksToSchedule(), ActiveDevice);
}

int ScheduleManager::getLastScheduleTime() const {
    return LastScheduleTime;
}
