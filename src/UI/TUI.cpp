//
// Created by moritz on 19.06.19.
//

#include "TUI.h"
#include <iostream>

using namespace UI;
using namespace std;

void TUI::start() {

    ScheduleManager = new SCHEDULER::ScheduleManager();
    ScheduleManager->searchForDevices();

    if(ScheduleManager->isAddingTasksPossible()){
        addKernelMenu();
    }else{
        decorateError("No devices Found! Abording.");
    }
}

void TUI::decorateError(std::string errorText) {
    system("clear");
    cout<<"\e[31m"+errorText<<endl;
}

void TUI::addKernelMenu() {
    std::string userInput="";
    while ((userInput.compare("no")!=0)&&((userInput.compare("n")!=0))){
        system("clear");
        cout<<"\e[0m Kernel count: "<<ScheduleManager->getKernelCount()<<endl;
        cout<<"\e[0m Adding Kernels: Please type in the FilePath:"<<endl;
        string filepath;
        cin>>filepath;
        //TODO CheckFilePath; //TODO Adding Dependancys CAN  and Handover
        cout<<"\e[0m Adding Kernels: Please type in the Kernel name:"<<endl;
        string kernelName;
        cin>>kernelName;
        ScheduleManager->addTask(filepath,kernelName);
        cout<<"\e[0m Adding Kernels: Do you want to add another Kernel (Yes/no)?"<<endl;
        cin>>userInput;
    }
}
