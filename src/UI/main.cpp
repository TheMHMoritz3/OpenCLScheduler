//
// Created by moritz on 17.06.19.
//

#include <iostream>
#include "../Scheduler/ScheduleManager.h"

using namespace std;
using namespace SCHEDULER;


int main(int count, char** values){
    if(count == 0){
        cout<<"No Arguments given Starting normal."<<endl;
    }

    ScheduleManager* scheduleManager = new ScheduleManager();
    scheduleManager->searchForDevices();

    return 0;
}