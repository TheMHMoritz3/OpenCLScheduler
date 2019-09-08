//
// Created by root on 20.07.19.
//

#include "DevicePropertys.h"
#include <iostream>
#include <sstream>

using namespace SCHEDULER;

DeviceProperties::DeviceProperties(std::string deviceName, int maxCoreCount) :
	DeviceName(deviceName),
	MaxCoreCount(maxCoreCount)
{
	Type = SERIAL;
	CoreCount = 1;
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
	if (Type != SERIAL)
		OutOfOrderExecution = ooe;
}

int DeviceProperties::getMaxCoreCount()
{
	return MaxCoreCount;
}

std::string DeviceProperties::toString()
{
	std::stringstream stream;

	switch (Type) {
	case SERIAL:
		stream << "Serial";
		break;
	    case STATIC:
	        stream<< "Static";
	        break;
	case ASAPHC: 
		stream << "ASAP";
		break;
	case READY_FIRE_SCHEDULER: 
		stream << "R. F. S.";
		break;
	default:;
	}

	if (getOutOfOrderExecution())
		stream << " OOE ";

	stream << " - Workgroup Size: " << getCoureCount();
    stream << " - Device: " << getName();
	return stream.str();
}

void DeviceProperties::clearTasksToSchedule() {
	TasksToSchedule.clear();
}

