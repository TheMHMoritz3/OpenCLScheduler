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

#include "ASAP.h"
#include <iostream>
#include <string>
#include <chrono>
#include <cstring>
#include <CL/cl.hpp>
#include <fstream>

using namespace SCHEDULER;
using namespace std;

ASAP::ASAP(std::vector<Task*> tasks, std::vector<Device*> device) : Scheduler(tasks, device) {
	ErrorCode = 1;
	if (device.size() <= 1)
		generateAllPrograms();
}


void ASAP::schedule() {
	for (Device* device : Devices) {
		cout << "Device Name: " << device->getName() << endl;
		cl::CommandQueue commandQueue(device->getDeviceContext(), device->getOclDevice());
		CommandQueues.push_back(commandQueue);

		TasksToSchedule = Tasks;

		while (!TasksToSchedule.empty()) {
			enqueueTasksWithNoDependency();
			while (!TasksToScheduleInStep.empty())
			{
				Task* task = TasksToScheduleInStep.front();
				TasksToScheduleInStep.pop();
				cl::Kernel kernel = cl::Kernel(task->getProgramm(), task->getKernelName().c_str(), &ErrorCode);
				if (ErrorCode == CL_SUCCESS) {
					setRAMForCurrentTask(task, device, kernel, commandQueue);
					readConstantsFromTask(task, device, kernel, commandQueue);
					setRAMBufferForOutput(task, device, kernel);
					setKernelLoad(task, device, kernel);
					enqueueTak(task, device, commandQueue, kernel);
				}
				else
					cout << "Kernel Creation Resolved Error: " << ErrorCode << endl;
			}
			commandQueue.finish();
			while (!TasksToReadInStep.empty()) {
				Task* task = TasksToReadInStep.front();
				TasksToReadInStep.pop();
				readDataFromTask(task, commandQueue);
			}
		}
	}
}

void ASAP::enqueueTasksWithNoDependency()
{
	int i = 0;
	for (Task* task : TasksToSchedule)
	{
		if (task->dependenciesAreCalculated())
		{
			TasksToScheduleInStep.push(task);
			TasksToSchedule.erase(TasksToSchedule.begin() + i);
		}
		i++;
	}
}

void ASAP::generateAllPrograms()
{
	for (Device* device : Devices)
		for (Task* task : Tasks)
			device->generateProgramm(task);
}

