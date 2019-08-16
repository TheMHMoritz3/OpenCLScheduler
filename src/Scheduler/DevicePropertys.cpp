//
// Created by root on 20.07.19.
//

#include "DevicePropertys.h"

using namespace SCHEDULER;

DeviceProperties::DeviceProperties (std::string deviceName) :
	DeviceName(deviceName)
{
	DeviceName = deviceName;
	Type = STATIC;
	CoreCount = 0;
}


std::string DeviceProperties::getName()
{
	return DeviceName;
}

int DeviceProperties::getCoureCount()
{
	return CoreCount;
}

void DeviceProperties::setSchedule(ScheduleType type)
{
	Type = type;
}

void DeviceProperties::addTaskToSchedule(Task* task)
{
	TasksToSchedule.push_back(task);
}
