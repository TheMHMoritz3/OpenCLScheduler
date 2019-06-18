//
// Created by moritz on 17.06.19.
//

#include "Device.h"

using namespace SCHEDULER;

Device::Device() {

}

std::string Device::getName() {
    return std::__cxx11::string();
}

int Device::scheduledTasks() {
    return 0;
}

int Device::getWorkgroupSize() {
    return 0;
}

int Device::getID() {
    return 0;
}

double Device::getDeviceUsage() {
    return 0;
}

void Device::schedule(Task tak) {

}
