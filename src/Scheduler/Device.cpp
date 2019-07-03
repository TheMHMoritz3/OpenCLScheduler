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

#include "Device.h"
#include <iostream>
#include "ScheduleManager.h"
using namespace std;
using namespace SCHEDULER;

Device::Device(int id, cl::Device oclDevice) :
OclDevice(oclDevice)
{
    OclContext =  cl::Context({OclDevice});
    Id=id;
}

Device::~Device() {

}

string Device::getName() {
    return OclDevice.getInfo<CL_DEVICE_NAME>();
}

int Device::scheduledTasks() {
    return 0;
}

int Device::getWorkgroupSize() {
    return OclDevice.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
}

int Device::getID() {
    return Id;
}

double Device::getDeviceUsage() {
    return 0.0;
}

int Device::getMaxComputeUnits() {
    return OclDevice.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
}

double Device::getComputeUnitUsage(int ComputeUnit) {
    return 0;
}

void Device::generateProgramm(Task* task) {

    cl::Program program(OclContext, *task->getSources());
	if (program.build({ OclDevice }) != CL_SUCCESS)
		cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(OclDevice) << endl;

    task->setProgam(program);
}

cl::Context Device::getDeviceContext() {
    return OclContext;
}

cl::Device Device::getOclDevice()
{
	return OclDevice;
}

Device Device::operator=(Device other) {
	OclDevice = other.OclDevice;
	OclContext = other.OclContext;
    return *this;
}
