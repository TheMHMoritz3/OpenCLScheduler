//
// Created by mhmoritz3 on 07.05.19.
//

#ifndef EMBEDDEDSYSTEMSPROJECT_INSTANCEMANAGER_H
#define EMBEDDEDSYSTEMSPROJECT_INSTANCEMANAGER_H

#include <CL/cl.hpp>

/**
 * Manages the Instance of the Programm
 */
class InstanceManager {
public:
    /**
     * Represents Integer of Core
     * VideoCore = 0
     * CPUCore = 1
     */
    enum Core {
        VideoCore,
        CPUCore
    };
    InstanceManager();
private:
    //Variables
    const int DEFAULT_SIZE;
    double ExecutionTimeVideoCore;
    double ExecutionTimeCPUCore;

    cl::Device CPUDevice;
    cl::Device VideoDevice;

    cl::Context CPUContext;
    cl::Context VideoContext;

    /**
     * Searches for the forst device of an Platform.
     * @param platform The korresbonding Platform like Videocore or CPUCore
     * @return The first Device of the given Platform.
     */
    cl::Device setUpDevice(Core platform);
};


#endif //EMBEDDEDSYSTEMSPROJECT_INSTANCEMANAGER_H
