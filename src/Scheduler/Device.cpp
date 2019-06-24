//
// Created by moritz on 17.06.19.
//

#include "Device.h"
#include <iostream>

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

void Device::schedule(Task tak, int computeUnit) {

}

int Device::getMaxComputeUnits() {
    return OclDevice.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
}

double Device::getComputeUnitUsage(int ComputeUnit) {
    return 0;
}

void Device::generateProgramm(Task* task) {

    cl::Program program(OclContext, *task->getSources());
    cout <<"Sources"<< task->getSources()->at(0).first<<endl;
	if(program.build({OclDevice})!=CL_SUCCESS)
		cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(OclDevice) << "\n";
	else
	    cout << "Success" <<endl;
    task->setProgam(program);
}

cl::Context Device::getDeviceContext() {
    return OclContext;
}

cl::Device SCHEDULER::Device::Device::getOclDevice()
{
	return OclDevice;
}

Device Device::operator=(Device other) {
    return other;
}
