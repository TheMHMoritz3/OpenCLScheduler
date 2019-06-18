//
// Created by moritz on 17.06.19.
//

#ifndef CAN_DEVICE_H
#define CAN_DEVICE_H

#include <string>
#include "Task.h"

namespace SCHEDULER {
    class Device {
    public:
        Device();
        std::string getName();
        int scheduledTasks();
        int getWorkgroupSize();
        int getID();
        double getDeviceUsage();
        void schedule(Task tak);
    };
}

#endif //CAN_DEVICE_H
