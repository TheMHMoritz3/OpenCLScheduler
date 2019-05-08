//
// Created by mhmoritz3 on 07.05.19.
//

#include "InstanceManager.h"
#include <iostream>

using namespace std;
using namespace cl;

InstanceManager::InstanceManager() :
    DEFAULT_SIZE(10),
    ExecutionTimeCPUCore(0),
    ExecutionTimeVideoCore(0)
{
    CPUDevice = setUpDevice(CPUCore);
    CPUContext = Context({CPUDevice});
    VideoDevice = setUpDevice(VideoCore);
    VideoContext = Context({VideoDevice});
}

Device InstanceManager::setUpDevice(Core platform) {
    vector<Platform> allPlattforms;
    char platformName[64];
    Platform::get(&allPlattforms);

    if(allPlattforms.empty()){
        cout<<"No Platform found. Please check OpenCL installation!"<<endl;
        exit(1);
    }

    Platform platformToUse = allPlattforms[platform];
    cout<<"Use platform: "<<platformToUse.getInfo<CL_PLATFORM_NAME>()<<endl;

    vector<Device> allDevices;
    platformToUse.getDevices(CL_DEVICE_TYPE_ALL,&allDevices);

    if(allDevices.empty())
    {
        cout<<"No Device found. Please check OpenCL installation!"<<endl;
        exit(1);
    }

    return allDevices[0]; //Only one Core of the Pi is used?
}
