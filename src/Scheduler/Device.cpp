//
// Created by moritz on 17.06.19.
//

#include "Device.h"
using namespace std;
using namespace SCHEDULER;

Device::Device(int id, cl::Device* oclDevice) :
OclDevice(oclDevice)
{
    Id=id;
}

Device::~Device() {
    delete OclDevice;
}

string Device::getName() {
    return OclDevice->getInfo<CL_DEVICE_NAME>();
}

int Device::scheduledTasks() {
    return 0;
}

int Device::getWorkgroupSize() {
    return OclDevice->getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
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
    return OclDevice->getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>();
}

double Device::getComputeUnitUsage(int ComputeUnit) {
    return 0;
}
