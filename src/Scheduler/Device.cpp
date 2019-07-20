//
// Created by moritz on 17.06.19.
//

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
	Properties = new DeviceProperties(getName());
}

Device::~Device() {
	delete Properties;
}

string Device::getName() const {
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

DeviceProperties* Device::getProperties() const
{
	return Properties;
}
