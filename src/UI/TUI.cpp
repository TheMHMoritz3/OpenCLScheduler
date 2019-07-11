/*
Embedded Systems Project 2019
Copyright (C) 2019  Moritz Herzog, Philip Lersch

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
    activateCanBus();
	ScheduleManager = nullptr;
    DefaultStaticModeLoad=10;
}

void TUI::start() {
	ScheduleManager = new SCHEDULER::ScheduleManager();
	ScheduleManager->searchForDevices();

	if (ScheduleManager->isAddingTasksPossible()) {
	    decorateNormalMessage("Do you want to use the Kernelpreset?\n(y/N)");
	    std::string presetMode;
	    cin>>presetMode;
	    if((presetMode.compare("y")==0)||(presetMode.compare("Y")==0)||(presetMode.compare("Yes")==0)||(presetMode.compare("yes")==0)){
	        applyStaticMode();
	    }else{
            addKernelMenu();
	    }
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
		decorateIntValue(task, task->getReturnData().second, task->getLoad());
		break;
	case SCHEDULER::UINT:
		decorateUIntValue(task, task->getReturnData().second, task->getLoad());
		break;
	case SCHEDULER::CHAR:
		decorateCharValue(task, task->getReturnData().second, task->getLoad());
		break;
	case SCHEDULER::FLOAT:
		decorateFloatValue(task, task->getReturnData().second, task->getLoad());
		break;
	case SCHEDULER::DOUBLE:
		decorateDoubleValue(task, task->getReturnData().second, task->getLoad());
		break;
	case SCHEDULER::STRING:
		decorateCharValue(task, task->getReturnData().second, task->getLoad());
		break;
	default:
		decorateError("Error: No Type provided in Return Value");
		break;
	}
}

void TUI::decorateFloatValue(SCHEDULER::Task* task, std::vector<void*> data, int load)
{
    cout<<"Data Size: "<<data.size()<<endl;
	for (long unsigned int i = 0; i < load; i++) {
		float value = *((float*)data.at(i));
		cout << i << ". Return Value: \t" << value << endl;
	}
}

void TUI::decorateIntValue(SCHEDULER::Task* task, std::vector<void*> data, int load)
{
	for (long unsigned int i = 0; i < load; i++) {
		int32_t value = *((int32_t*)data.at(i));
		cout << i << ". Return Value: \t" << value << endl;
	}
}

void TUI::decorateUIntValue(SCHEDULER::Task* task, std::vector<void*> data, int load)
{
	for (long unsigned int i = 0; i < load; i++) {
		uint32_t value = *((uint32_t*)data.at(i));
		cout << i << ". Return Value: \t" << value << endl;
	}
}

void TUI::decorateCharValue(SCHEDULER::Task* task, std::vector<void*> data, int load)
{
	for (long unsigned int i = 0; i < load; i++) {
		char value = *((char*)data.at(i));
		cout << i << ". Return Value: \t" << value << endl;
	}
}

void TUI::decorateDoubleValue(SCHEDULER::Task* task, std::vector<void*> data, int load)
{
	for (long unsigned int i = 0; i < load; i++) {
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
		if(i==value)
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

	addCanMethod(task,load,value);
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

void TUI::applyStaticMode() {
    activateCanBus();
    clear();
    cout << "Found Devices: " << ScheduleManager->getDeviceCount() << endl;
    //Task xAxis
    decorateNormalMessage("xAxis");
    SCHEDULER::Task* xAxis = ScheduleManager->addTask("kernels/accel_sensor.cl", "xAxis");
    xAxis->setReturnDataType(SCHEDULER::Type::FLOAT);
    xAxis->setLoad(DefaultStaticModeLoad);
    xAxis->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    addCanMethod(xAxis,DefaultStaticModeLoad,5);
    tasks.emplace_back(xAxis);
    //Task yAxis
    decorateNormalMessage("yAxis");
    SCHEDULER::Task* yAxis = ScheduleManager->addTask("kernels/accel_sensor.cl", "yAxis");
    yAxis->setReturnDataType(SCHEDULER::Type::FLOAT);
    yAxis->setLoad(DefaultStaticModeLoad);
    yAxis->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    addCanMethod(yAxis,DefaultStaticModeLoad,6);
    tasks.emplace_back(yAxis);
    //Task dualAxis
    decorateNormalMessage("dualAxis");
    SCHEDULER::Task* dualAxis = ScheduleManager->addTask("kernels/accel_sensor.cl", "dualAxis");
    dualAxis->setReturnDataType(SCHEDULER::Type::FLOAT);
    dualAxis->setLoad(DefaultStaticModeLoad);
    dualAxis->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    addCanMethod(dualAxis,DefaultStaticModeLoad,5);
    addCanMethod(dualAxis,DefaultStaticModeLoad,6);
    tasks.emplace_back(dualAxis);
    //Task batteryCalc
    decorateNormalMessage("batteryCalc");
    SCHEDULER::Task* batteryCalc = ScheduleManager->addTask("kernels/battery_kernel.cl", "batteryCalc");
    batteryCalc->setReturnDataType(SCHEDULER::Type::FLOAT);
    batteryCalc->setLoad(DefaultStaticModeLoad);
    batteryCalc->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    addCanMethod(batteryCalc,DefaultStaticModeLoad,4);
    tasks.emplace_back(batteryCalc);
    //Task temp
    decorateNormalMessage("temp");
    SCHEDULER::Task* temp = ScheduleManager->addTask("kernels/temp_kernel.cl", "temp");
    temp->setReturnDataType(SCHEDULER::Type::FLOAT);
    temp->setLoad(DefaultStaticModeLoad);
    temp->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    addCanMethod(temp,DefaultStaticModeLoad,7);
    tasks.emplace_back(temp);
    //Task speedCalcFR
    decorateNormalMessage("speedCalcFR");
    SCHEDULER::Task* speedCalcFR = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
    speedCalcFR->setReturnDataType(SCHEDULER::Type::FLOAT);
    speedCalcFR->setLoad(DefaultStaticModeLoad);
    speedCalcFR->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    addCanMethod(speedCalcFR,DefaultStaticModeLoad,0);
    tasks.emplace_back(speedCalcFR);
//    //Task speedCalcFL
//    decorateNormalMessage("speedCalcFL");
//    SCHEDULER::Task* speedCalcFL = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
//    speedCalcFL->setReturnDataType(SCHEDULER::Type::FLOAT);
//    speedCalcFL->setLoad(DefaultStaticModeLoad);
//    speedCalcFL->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
//    addCanMethod(speedCalcFL,DefaultStaticModeLoad,1);
//    tasks.emplace_back(speedCalcFL);
    //Task speedCalcRR
    decorateNormalMessage("speedCalcRR");
    SCHEDULER::Task* speedCalcRR = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
    speedCalcRR->setReturnDataType(SCHEDULER::Type::FLOAT);
    speedCalcRR->setLoad(DefaultStaticModeLoad);
    speedCalcRR->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
    addCanMethod(speedCalcRR,DefaultStaticModeLoad,3);
    tasks.emplace_back(speedCalcRR);
//    //Task speedCalcRL
//    decorateNormalMessage("speedCalcRL");
//    SCHEDULER::Task* speedCalcRL = ScheduleManager->addTask("kernels/speed_kernel.cl", "speedCalc");
//    speedCalcRL->setReturnDataType(SCHEDULER::Type::FLOAT);
//    speedCalcRL->setLoad(DefaultStaticModeLoad);
//    speedCalcRL->setDataDependancy(SCHEDULER::DependancyType::OutsideDependancy);
//    addCanMethod(speedCalcRL,DefaultStaticModeLoad,2);
//    tasks.emplace_back(speedCalcRL);
    //Task median
    SCHEDULER::Task* median = ScheduleManager->addTask("kernels/speed_kernel.cl", "median");
    median->setReturnDataType(SCHEDULER::Type::FLOAT);
    median->setLoad(DefaultStaticModeLoad);
    median->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    median->addDependandTask(speedCalcRR);
    median->addDependandTask(speedCalcFR);
    tasks.emplace_back(median);
    //Task range
    decorateNormalMessage("range");
    SCHEDULER::Task* range = ScheduleManager->addTask("kernels/range_kernel.cl", "dist");
    range->setReturnDataType(SCHEDULER::Type::FLOAT);
    range->setLoad(DefaultStaticModeLoad);
    range->setDataDependancy(SCHEDULER::DependancyType::OtherTask);
    range->addDependandTask(batteryCalc);
    range->addDependandTask(median);
    tasks.emplace_back(range);


    cout << "Kernel count: " << ScheduleManager->getKernelCount() << endl;
}

void TUI::addCanMethod(SCHEDULER::Task *task, int load, int value) {
    switch (value)
    {
        case 0:
            CanManager->create(CAN::WheelFrontRight, load);
            task->addExternalDataMethod([=]()
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
            task->addExternalDataMethod([=]()
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
            task->addExternalDataMethod([=]()
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
            task->addExternalDataMethod([=]()
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
            task->addExternalDataMethod([=]()
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
            task->addExternalDataMethod([=]()
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
            task->addExternalDataMethod([=]()
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
            task->addExternalDataMethod([=]()
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
