//
// Created by moritz on 17.06.19.
//

#include "CanManager.h"

using namespace std;
using namespace CAN;

CanManager::CanManager() {

}

void CanManager::create(CAN::CanID id, int count) {
    CanAccessor *accessor = new CanAccessor(id, count);
    accessor->startCollectingData();
    CanThreadMap.insert(pair<int, CanAccessor*>(id,accessor));
}

void CanManager::create(std::vector<CAN::CanID> ids, int count) {
    for(CanID id : ids){
        CanAccessor *accessor = new CanAccessor(id, count);
        accessor->startCollectingData();
        CanThreadMap.insert(pair<int, CanAccessor*>(id,accessor));
    }
}

double CanManager::getSamplingRate(CAN::CanID id) {
    switch (id){
        case CanID::WheelFrontRight:
        case CanID::WheelFrontLeft:
        case CanID::WheelRearLeft:
        case CanID::WheelRearRight:
        case CanID::AccelerationLateral:
        case CanID::AccelerationLongitudinal:
        case CanID::Temperature:
            return 0.050;
        case CanID::BatteryVoltage:
            return 0.2;
        default:
            return 0.0;
    }
}

vector<uint32_t*> CanManager::getData(CAN::CanID id) {
    return CanThreadMap.at(id)->getData();
}
