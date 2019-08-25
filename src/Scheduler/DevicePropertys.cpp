//
// Created by root on 20.07.19.
//

#include "DevicePropertys.h"
#include <iostream>

using namespace SCHEDULER;

DeviceProperties::DeviceProperties (std::string deviceName) :
	DeviceName(deviceName)
{
	DeviceName = deviceName;
	Type = STATIC;
	CoreCount = 0;
	OutOfOrderExecution = false;
}


std::string DeviceProperties::getName()
{
	return DeviceName;
}

int DeviceProperties::getCoureCount()
{
	return CoreCount;
}

void DeviceProperties::setCoreCount(int coreCount)
{
	CoreCount = coreCount;
}

void DeviceProperties::setSchedule(ScheduleType type)
{
	Type = type;
}

ScheduleType DeviceProperties::getSchedule()
{
	return Type;
}

std::vector<Task*> DeviceProperties::getTasksToSchedule()
{
	return TasksToSchedule;
}

void DeviceProperties::addTaskToSchedule(Task* task)
{
	TasksToSchedule.push_back(task);
}

bool DeviceProperties::getOutOfOrderExecution() {
    return OutOfOrderExecution;
}

void DeviceProperties::setOutOfOrderExecution(bool ooe) {
    if(Type != STATIC)
        OutOfOrderExecution=ooe;
}
