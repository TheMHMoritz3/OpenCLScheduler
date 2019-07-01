//
// Created by moritz on 19.06.19.
//

#include "TUI.h"
#include "../Scheduler/Task.h"
#include "../CAN/CanNamespace.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace UI;
using namespace std;

TUI::TUI()
{
	IsInUnitTestingMode = false;
	CanManager = nullptr;
	ScheduleManager = nullptr;
}

void TUI::start() {
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
		cout << "Found Devices: " << ScheduleManager->getDeviceCount() << endl;
		cout << "Kernel count: " << ScheduleManager->getKernelCount() << endl;
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
		decorateNormalMessage("Adding Kernels: Please type in the Kernel name:");
		string kernelName;
		cin >> kernelName;
		SCHEDULER::Task* task = ScheduleManager->addTask(filepath, kernelName);
		askUserReturnData(task);
		if (IsInUnitTestingMode)
			decorateUnitTestingMode(task);
		else
			decorateNormalMode(task);

		cout << "Adding Kernels: Do you want to add another Kernel (Yes/no)?" << endl;
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
	decorateNormalMessage("Printing Data of: " + task->getKernelName());
	switch (task->getReturnDataType())
	{
	case SCHEDULER::INT:
		decorateIntValue(task, task->getReturnData().second);
		break;
	case SCHEDULER::UINT:
		decorateUIntValue(task, task->getReturnData().second);
		break;
	case SCHEDULER::CHAR:
		decorateCharValue(task, task->getReturnData().second);
		break;
	case SCHEDULER::FLOAT:
		decorateFloatValue(task, task->getReturnData().second);
		break;
	case SCHEDULER::DOUBLE:
		decorateDoubleValue(task, task->getReturnData().second);
		break;
	case SCHEDULER::STRING:
		decorateCharValue(task, task->getReturnData().second);
		break;
	default:
		decorateError("Error: No Type providet in Return Value");
		break;
	}
}

void TUI::decorateFloatValue(SCHEDULER::Task* task, std::vector<void*> data)
{
	for (long unsigned int i = 0; i < data.size(); i++) {
		float value = *((float*)data.at(i));
		cout << i << ". Return Value: \t" << value << endl;
	}
}

void TUI::decorateIntValue(SCHEDULER::Task* task, std::vector<void*> data)
{
	for (long unsigned int i = 0; i < data.size(); i++) {
		int32_t value = *((int32_t*)data.at(i));
		cout << i << ". Return Value: \t" << value << endl;
	}
}

void TUI::decorateUIntValue(SCHEDULER::Task* task, std::vector<void*> data)
{
	for (long unsigned int i = 0; i < data.size(); i++) {
		uint32_t value = *((uint32_t*)data.at(i));
		cout << i << ". Return Value: \t" << value << endl;
	}
}

void TUI::decorateCharValue(SCHEDULER::Task* task, std::vector<void*> data)
{
	for (long unsigned int i = 0; i < data.size(); i++) {
		char value = *((char*)data.at(i));
		cout << i << ". Return Value: \t" << value << endl;
	}
}

void TUI::decorateDoubleValue(SCHEDULER::Task* task, std::vector<void*> data)
{
	for (long unsigned int i = 0; i < data.size(); i++) {
		double value = *((double*)data.at(i));
		cout << i << ". Return Value: \t" << value << endl;
	}
}

void TUI::decorateOtherTask(SCHEDULER::Task* task)
{
	int i = 0;
	int value;
	do {
		decorateNormalMessage("Which task do you want to add?");
		for (SCHEDULER::Task* task : tasks)
		{
			cout << i << "\t" << task->getKernelName()<<endl;
			i++;
		}
		cin >> value;
		if (!((value >= 0) && (value < tasks.size())))
		{
			clear();
			decorateError("Please Type in a correct Value.");
		}
	} while (!((value >= 0) && (value < tasks.size())));

	for(int i =0 ;i<tasks.size(); i++)
	{
		if(i=value)
		{
			task->addDependandTask(tasks.at(i));
		}
	}
}

void TUI::decorateCan(SCHEDULER::Task* task, int load)
{
	clear();
	int value;
	do {
		decorateNormalMessage("Which CAN ID do you want to add?");
		decorateNormalMessage("0\t Wheel Front Right");
		decorateNormalMessage("1\t Wheel Front Left");
		decorateNormalMessage("2\t Wheel Rear Left");
		decorateNormalMessage("3\t Wheel Rear Right");
		decorateNormalMessage("4\t Battery Voltage");
		decorateNormalMessage("5\t Acceleration Longitudinal");
		decorateNormalMessage("6\t Acceleration Lateral");
		decorateNormalMessage("7\t Temperature");
		cin >> value;
		if (!((value >= 0) && (value <= 7)))
		{
			clear();
			decorateError("Please Type in a correct Value.");
		}
	} while (!((value >= 0) && (value <= 7)));

	switch (value)
	{
	case 0:
		CanManager->create(CAN::WheelFrontRight, load);
		task->setExternalDataMethod([=]()
			{
				std::vector<uint32_t*> dataSet  = CanManager->getData(CAN::WheelFrontRight);
				uint32_t* DataSet = new uint32_t[dataSet.size()];
				std::vector<void*> taskData;
				int i = 0;
				for(uint32_t* data : dataSet)
				{
				    DataSet[i]=*data;
					taskData.push_back(&DataSet[i]);
				    i++;
				}
				task->addData(taskData, SCHEDULER::UINT);
			});
		break;
	case 1:
		CanManager->create(CAN::WheelFrontLeft, load);
		task->setExternalDataMethod([=]()
			{
				std::vector<uint32_t*> dataSet = CanManager->getData(CAN::WheelFrontLeft);
                uint32_t* DataSet = new uint32_t[dataSet.size()];
                std::vector<void*> taskData;
                int i = 0;
                for(uint32_t* data : dataSet)
                {
                    DataSet[i]=*data;
                    taskData.push_back(&DataSet[i]);
                    i++;
                }
				task->addData(taskData, SCHEDULER::UINT);
			});
		break;
	case 2:
		CanManager->create(CAN::WheelRearLeft, load);
		task->setExternalDataMethod([=]()
			{
				std::vector<uint32_t*> dataSet = CanManager->getData(CAN::WheelRearLeft);
                uint32_t* DataSet = new uint32_t[dataSet.size()];
                std::vector<void*> taskData;
                int i = 0;
                for(uint32_t* data : dataSet)
                {
                    DataSet[i]=*data;
                    taskData.push_back(&DataSet[i]);
                    i++;
                }
				task->addData(taskData, SCHEDULER::UINT);
			});
		break;
	case 3:
		CanManager->create(CAN::WheelRearRight, load);
		task->setExternalDataMethod([=]()
			{
				std::vector<uint32_t*> dataSet = CanManager->getData(CAN::WheelRearRight);
                uint32_t* DataSet = new uint32_t[dataSet.size()];
                std::vector<void*> taskData;
                int i = 0;
                for(uint32_t* data : dataSet)
                {
                    DataSet[i]=*data;
                    taskData.push_back(&DataSet[i]);
                    i++;
                }
				task->addData(taskData, SCHEDULER::UINT);
			});
		break;
	case 4:
		CanManager->create(CAN::BatteryVoltage, load);
		task->setExternalDataMethod([=]()
			{
				std::vector<uint32_t*> dataSet = CanManager->getData(CAN::BatteryVoltage);
                uint32_t* DataSet = new uint32_t[dataSet.size()];
                std::vector<void*> taskData;
                int i = 0;
                for(uint32_t* data : dataSet)
                {
                    DataSet[i]=*data;
                    taskData.push_back(&DataSet[i]);
                    i++;
                }
				task->addData(taskData, SCHEDULER::UINT);
			});
		break;
	case 5:
		CanManager->create(CAN::AccelerationLongitudinal, load);
		task->setExternalDataMethod([=]()
			{
				std::vector<uint32_t*> dataSet = CanManager->getData(CAN::AccelerationLongitudinal);
                uint32_t* DataSet = new uint32_t[dataSet.size()];
                std::vector<void*> taskData;
                int i = 0;
                for(uint32_t* data : dataSet)
                {
                    DataSet[i]=*data;
                    taskData.push_back(&DataSet[i]);
                    i++;
                }
				task->addData(taskData, SCHEDULER::UINT);
			});
		break;
	case 6:
		CanManager->create(CAN::AccelerationLateral, load);
		task->setExternalDataMethod([=]()
			{
				std::vector<uint32_t*> dataSet = CanManager->getData(CAN::AccelerationLateral);
                uint32_t* DataSet = new uint32_t[dataSet.size()];
                std::vector<void*> taskData;
                int i = 0;
                for(uint32_t* data : dataSet)
                {
                    DataSet[i]=*data;
                    taskData.push_back(&DataSet[i]);
                    i++;
                }
				task->addData(taskData, SCHEDULER::UINT);
			});
		break;
	case 7:
		CanManager->create(CAN::Temperature, load);
		task->setExternalDataMethod([=]()
			{
				std::vector<uint32_t*> dataSet = CanManager->getData(CAN::Temperature);
                uint32_t* DataSet = new uint32_t[dataSet.size()];
                std::vector<void*> taskData;
                int i = 0;
                for(uint32_t* data : dataSet)
                {
                    DataSet[i]=*data;
                    taskData.push_back(&DataSet[i]);
                    i++;
                }
				task->addData(taskData, SCHEDULER::UINT);
			});
		break;
	default:
		break;
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
		if (!((value >= 0) && (value <= 5)))
		{
			clear();
			decorateError("Please Type in a correct Value.");
		}
	} while (!((value >= 0) && (value <= 5)));
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
		cout << "Setting Args of Array: " << i << endl;
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
	std::vector<void*> data;

	switch (type) {
	case SCHEDULER::INT:
		data = askUserForIntegerArray(load);
		break;
	case SCHEDULER::UINT:
		data = askUserForUIntegerArray(load, task);
		break;
	case SCHEDULER::CHAR:
		data = askUserForCharArray(load);
		break;
	case SCHEDULER::FLOAT:
		data = askUserForFloatArray(load);
		break;
	case SCHEDULER::DOUBLE:
		data = askUserForDoubleArray(load);
		break;
	case SCHEDULER::STRING:
		data = askUserForCharArray(load);
		break;
	default:
		decorateError("Wrong Type given.");
		break;
	}

	task->addData(data, type);
}

std::vector<void*> TUI::askUserForIntegerArray(int load)
{
	std::vector<void*> returnData;
	int32_t* values = new int32_t[load];
	for (int j = 0; j < load; j++) {
		cout << j << ". Integer Entry: " << endl;
		int value;
		cin >> value;
		values[j] = value;
		returnData.emplace_back(&value);
	}
	return returnData;
}

std::vector<void*> TUI::askUserForUIntegerArray(int load, SCHEDULER::Task* task)
{
	std::vector<void*> returnData;
	uint32_t* values = new uint32_t[load];
	for (int j = 0; j < load; j++) {
		cout << j << ". U-Integer Entry: " << endl;
		uint32_t value;
		cin >> value;
		values[j] = value;
		cout << value << endl;
		returnData.emplace_back(&values[j]);
		cout << &value << endl;
		returnData.emplace_back(&values[j]);
	}
	return returnData;
}

std::vector<void*> TUI::askUserForCharArray(int load)
{
	std::vector<void*> returnData;
	char* values = new char[load];
	for (int j = 0; j < load; j++) {
		cout << j << ". Char Entry: " << endl;
		char value;
		cin >> value;
		values[j] = value;
		returnData.emplace_back(&values[j]);
	}
	return returnData;
}

std::vector<void*> TUI::askUserForDoubleArray(int load)
{
	std::vector<void*> returnData;
	double* values = new double[load];
	for (int j = 0; j < load; j++) {
		cout << j << ". Double Entry: " << endl;
		double value;
		cin >> value;
		values[j] = value;
		returnData.emplace_back(&values[j]);
	}
	return returnData;
}

std::vector<void*> TUI::askUserForFloatArray(int load)
{
	std::vector<void*> returnData;
	float* values = new float[load];
	for (int j = 0; j < load; j++) {
		cout << j << ". Float Entry: " << endl;
		float value;
		cin >> value;
		values[j] = value;
		returnData.emplace_back(&values[j]);
	}
	return returnData;
}

void TUI::toggleUnitTestMode() {
	IsInUnitTestingMode = true;
	decorateNormalMessage("Is in UnitTesting Mode");
}

void TUI::decorateNormalMode(SCHEDULER::Task* task) {
	activateCanBus();
	decorateNormalMessage("How many Arguments do you want to set for Task?");
	int argCount;
	cin >> argCount;
	for(int i = 0; i<argCount; i++)
	{
		int value;
		do {
			cout << "ARG: " << i <<endl;
			decorateNormalMessage("Where does the Data from this Task come from?");
			decorateNormalMessage("0\tUser Input");
			decorateNormalMessage("1\tOther Task");
			decorateNormalMessage("2\tCAN Bus");
			cin >> value;
			if (!((value >= 0) && (value <= 2)))
			{
				clear();
				decorateError("Please Type in a correct Value.");
			}
		} while (!((value >= 0) && (value <= 2)));
		task->setDataDependancy((SCHEDULER::DependancyType)value);

		int load;
		decorateNormalMessage("How big is the Array of Arguments?");
		cin >> load;

		if (task->dependancyType() == SCHEDULER::UserInput) {
			SCHEDULER::Type type = getTypeFromUserForArg();
			askUserForArrayData(task, type, load);
			task->setLoad(load);
		}else if(task->dependancyType() == SCHEDULER::OutsideDependancy)
		{
			decorateCan(task, load);
			task->setLoad(load);
		}else
		{
			decorateOtherTask(task);
		}
		
	}
	tasks.emplace_back(task);
	
}

void TUI::activateCanBus() {
	CanManager = new CAN::CanManager();
}
