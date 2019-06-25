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
#ifdef _WINDOWS
	cout << errorText << endl;
#else
	cout << "\e[31m" + errorText << endl;
#endif // _WINDOWS
}

void TUI::decorateNormalMessage(std::string message)
{
#ifdef _WINDOWS
	cout << message << endl;
#else
	cout << "\e[0m" << message << endl;
#endif // _WINDOWS

}

void TUI::addKernelMenu() {
	std::string userInput = "";
	while ((userInput.compare("no") != 0) && ((userInput.compare("n") != 0))) {
		clear();
		cout << "Found Devices: " << ScheduleManager->getDeviceCount()<<endl;
		cout << "Kernel count: " << ScheduleManager->getKernelCount()<<endl;
		bool fileIsOpen = false;
		string filepath;
		do {
			decorateNormalMessage("Adding Kernels: Please type in the FilePath:");
			cin >> filepath;
			ifstream sourceFile;
			sourceFile.open(filepath);
			fileIsOpen = sourceFile.is_open();
			if (!fileIsOpen)
				decorateError(filepath + " File could not be opened");
		} while (!fileIsOpen);
		//TODO Adding Dependancys CAN  and Handover
		decorateNormalMessage("Adding Kernels: Please type in the Kernel name:");
		string kernelName;
		cin >> kernelName;
		SCHEDULER::Task* task = ScheduleManager->addTask(filepath, kernelName);
		askUserReturnData(task);
		if (IsInUnitTestingMode)
			decorateUnitTestingMode(task);

		decorateNormalMessage("Adding Kernels: Do you want to add another Kernel (Yes/no)?");
		cin >> userInput;
	}
}

void TUI::setSchedule()
{
	clear();
	decorateNormalMessage("Which Scheduling Type do you want to use?");
	decorateNormalMessage("1\t Static Scheduler");
	int userInput;
	cin >> userInput;
	if (userInput == 1)
		ScheduleManager->setScheduleType(SCHEDULER::ScheduleType::STATIC);
	ScheduleManager->startSchedule();
}

void TUI::printData()
{
	for (SCHEDULER::Task* task : tasks) {
        decorateValueData(task);
	}
}

void TUI::clear()
{
#ifdef _WINDOWS
	system("cls");
#else
	system("clear");
#endif // !_WINDOWS

}

void TUI::decorateValueData(SCHEDULER::Task* task)
{
	switch (task->getReturnDataType())
	{
	case SCHEDULER::INT:
		decorateIntValue(task,task->getReturnData().second);
		break;
	case SCHEDULER::UINT:
		decorateUIntValue(task,task->getReturnData().second);
		break;
	case SCHEDULER::CHAR:
		decorateCharValue(task,task->getReturnData().second);
		break;
	case SCHEDULER::FLOAT:
		decorateFloatValue(task,task->getReturnData().second);
		break;
	case SCHEDULER::DOUBLE:
		decorateDoubleValue(task,task->getReturnData().second);
		break;
	case SCHEDULER::STRING:
		decorateCharValue(task,task->getReturnData().second);
		break;
	default:
		decorateError("Error: No Type providet in Return Value");
		break;
	}
}

void TUI::decorateFloatValue(SCHEDULER::Task* task,void* data)
{
	float* value = (float*)data;
    for (int i = 0; i < task->getLoad(); i++) {
        cout << i << ". Return Value: \t"<<value[i]<<endl;

    }
}

void TUI::decorateIntValue(SCHEDULER::Task* task,void* data)
{
	uint32_t* value = (uint32_t*)data;
    for (int i = 0; i < task->getLoad(); i++) {
        cout << i << ". Return Value: \t"<<value[i]<<endl;

    }
}

void TUI::decorateUIntValue(SCHEDULER::Task* task,void* data)
{
	uint32_t* value = (uint32_t*)data;
    for (int i = 0; i < task->getLoad(); i++) {
        cout << i << ". Return Value: \t"<<value[i]<<endl;

    }
}

void TUI::decorateCharValue(SCHEDULER::Task* task,void* data)
{
	char* value = (char*)data;
    for (int i = 0; i < task->getLoad(); i++) {
        cout << i << ". Return Value: \t"<<value[i]<<endl;

    }
}

void TUI::decorateDoubleValue(SCHEDULER::Task* task,void* data)
{
	double* value = (double*)data;
    for (int i = 0; i < task->getLoad(); i++) {
        cout << i << ". Return Value: \t"<<value[i]<<endl;

    }
}


void TUI::askUserReturnData(SCHEDULER::Task* task)
{
	clear();
	int value;
	do {
		decorateNormalMessage("Witch return type is your Kernel of?");
		decorateNormalMessage("0\t Integer");
		decorateNormalMessage("1\t Unsinged Integer");
		decorateNormalMessage("2\t Charakter");
		decorateNormalMessage("3\t Float");
		decorateNormalMessage("4\t Double");
		decorateNormalMessage("5\t String");
		cin >> value;
		if (!((value >= 0) && (value <= 10)))
		{
			clear();
			decorateError("Please Type in a correct Value.");
		}
	} while (!((value >= 0) && (value <= 10)));
	task->setReturnDataType((SCHEDULER::Type)value);
}

void TUI::decorateUnitTestingMode(SCHEDULER::Task* task)
{
	decorateNormalMessage("How many Arguments do you want to set?");
	int argcount;
	cin >> argcount;
	int load;
	decorateNormalMessage("How big is the Array of Arguments?");
	cin >> load;
	task->setLoad(load);
	task->setReturnDataType(SCHEDULER::Type::FLOAT);
	for (int i = 0; i < argcount; i++) {
		clear();
		decorateNormalMessage("Setting Args of Array: " + i);
		SCHEDULER::Type type = getTypeFromUserForArg();
		askUserForArrayData(task, type, load);
	}
	tasks.push_back(task);
}

SCHEDULER::Type TUI::getTypeFromUserForArg()
{
	clear();
	int value;
	do {
		decorateNormalMessage("Witch type is this Arg of?");
		decorateNormalMessage("0\t Integer");
		decorateNormalMessage("1\t Unsinged Integer");
		decorateNormalMessage("2\t Charakter");
		decorateNormalMessage("3\t Float");
		decorateNormalMessage("4\t Double");
		decorateNormalMessage("5\t String");
		cin >> value;
		if (!((value >= 0) && (value <= 10)))
		{
			clear();
			decorateError("Please Type in a correct Value.");
		}
	} while (!((value >= 0) && (value <= 10)));
	return (SCHEDULER::Type)value;
}

void TUI::askUserForArrayData(SCHEDULER::Task* task, SCHEDULER::Type type, int load)
{
	clear();
	void* data = NULL;

	switch (type) {
	case SCHEDULER::INT:
		askUserForIntegerArray(load);
		break;
	case SCHEDULER::UINT:
		askUserForUIntegerArray(load);
		break;
	case SCHEDULER::CHAR:
		askUserForCharArray(load);
		break;
	case SCHEDULER::FLOAT:
		askUserForFloatArray(load);
		break;
	case SCHEDULER::DOUBLE:
		askUserForDoubleArray(load);
		break;
	case SCHEDULER::STRING:
		askUserForCharArray(load);
		break;
	default:
		decorateError("Wrong Type given.");
		break;
	}

	task->addData(data, type);
}

void* TUI::askUserForIntegerArray(int load)
{
	int32_t* values = new int32_t[load];
	for (int j = 0; j < load; j++) {
		decorateNormalMessage("Entry: " + j);
		int value;
		cin >> value;
		values[j] = value;
	}
	return values;
}

void* TUI::askUserForUIntegerArray(int load)
{
	uint32_t* values = new uint32_t[load];
	for (int j = 0; j < load; j++) {
		decorateNormalMessage("Entry: " + j);
		int value;
		cin >> value;
		values[j] = value;
	}
	return values;
}

void* TUI::askUserForCharArray(int load)
{
	char* values = new char[load];
	for (int j = 0; j < load; j++) {
		decorateNormalMessage("Entry: " + j);
		int value;
		cin >> value;
		values[j] = value;
	}
	return values;
}

void* TUI::askUserForDoubleArray(int load)
{
	double* values = new double[load];
	for (int j = 0; j < load; j++) {
		decorateNormalMessage("Entry: " + j);
		int value;
		cin >> value;
		values[j] = value;
	}
	return values;
}

void* TUI::askUserForFloatArray(int load)
{
	float* values = new float[load];
	for (int j = 0; j < load; j++) {
		decorateNormalMessage("Entry: " + j);
		int value;
		cin >> value;
		values[j] = value;
	}
	return values;
}
