//
// Created by moritz on 19.06.19.
//

#include "TUI.h"
#include "../Scheduler/Task.h"
#include <iostream>
#include <fstream>

using namespace UI;
using namespace std;

void TUI::start() {
	IsInUnitTestingMode = true;
	ScheduleManager = new SCHEDULER::ScheduleManager();
	ScheduleManager->searchForDevices();

	if (ScheduleManager->isAddingTasksPossible()) {
		addKernelMenu();
		setSchedule();
		printData();
	}
	else {
		decorateError("No devices Found! Abording.");
	}
}

void TUI::decorateError(std::string errorText) {
	clear();
	cout << "\e[31m" + errorText << endl;
}

void TUI::addKernelMenu() {
	std::string userInput = "";
	while ((userInput.compare("no") != 0) && ((userInput.compare("n") != 0))) {
		clear();
		cout << "Found Devices: " << ScheduleManager->getDeviceCount() << endl;
		cout << "\e[0m Kernel count: " << ScheduleManager->getKernelCount() << endl;
		bool fileIsOpen = false;
		string filepath;
		do {
			cout << "\e[0m Adding Kernels: Please type in the FilePath:" << endl;
			cin >> filepath;
			ifstream sourceFile;
			sourceFile.open(filepath);
			fileIsOpen = sourceFile.is_open();
			if (!fileIsOpen)
				decorateError(filepath + "file could not be opened");
		} while (fileIsOpen);
		//TODO Adding Dependancys CAN  and Handover
		cout << "\e[0m Adding Kernels: Please type in the Kernel name:" << endl;
		string kernelName;
		cin >> kernelName;
		SCHEDULER::Task task = ScheduleManager->addTask(filepath, kernelName);

		if (IsInUnitTestingMode) {
			cout << "How many Arguments do you want to set?" << endl;
			int argcount;
			cin >> argcount;
			int load;
			cout << "How big is the Array of Arguments?" << endl;
			cin >> load;
			task.setLoad(load);
			for (int i = 0; i < argcount; i++) {
				clear();
				cout << "Setting Args of "<<i<<". Array"<<endl;
				//TODO Float only for Testing
				float* values = new float[load];
				for (int j = 0; j < load; j++) {
					cout << j << ". Entry" << endl;
					float value;
					cin >> value;
					values[j] = value;
				}
				task.addData(values, SCHEDULER::Type::FLOAT);
			}
			tasks.push_back(task);
		}

		cout << "\e[0m Adding Kernels: Do you want to add another Kernel (Yes/no)?" << endl;
		cin >> userInput;
	}
}

void TUI::setSchedule()
{
	clear();
	cout << "\e[0m Which Scheduling Type do you want to use?" << endl;
	cout << "\e[0m 1\t Static Scheduler" << endl;
	int userInput;
	if (userInput == 1)
		ScheduleManager->setScheduleType(SCHEDULER::ScheduleType::STATIC);
}

void TUI::printData()
{
	clear();
	for (SCHEDULER::Task task : tasks) {
		//TODO Remove Float
		float* values;
		values = (float*)task.getReturnData().second;
		for (int i = 0; i < task.getLoad(); i++) {
			cout << i << ". Return Value: \t" << values[i] << endl;
		}
	}
}

void TUI::clear()
{
#ifdef _WINDOWS
	system("cls");
#endif
#ifndef _WINDOWS
	system("clear");
#endif // !_WINDOWS

}


