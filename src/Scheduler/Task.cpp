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

#include "Task.h"
#include <iostream>
#include "KernelFileParser.h"

using namespace SCHEDULER;


Task::Task(int id) {
    ID=id;
	IsCalculationDone = false;
    IsDataSet = false;
	ElapsedTime = 0.0;
}

int Task::getId() {
    return ID;
}

void Task::setKernel(std::string kernelName) {
    KernelName = kernelName;
}

void Task::setPath(std::string path)
{
	Path = path;
}

void Task::setLoad(int load)
{
	Load = load;
}

int Task::getLoad() {
	return Load;
}

void Task::addDescription(std::string desc) {
    Description=desc;
}

void Task::setProgramSources(cl::Program::Sources* sources) {
    Sources=sources;
}

void Task::setProgam(cl::Program program) {
    Program=program;
}

cl::Program::Sources* Task::getSources() {
    return Sources;
}

void Task::setReadBuffer(cl::Buffer *readBuffer)
{
	ReadBuffer = readBuffer;
}

cl::Buffer* Task::readBuffer()
{
	return ReadBuffer;
}

cl::Program Task::getProgramm()
{
	return Program;
}

std::string Task::getDescription() {
    return Description;
}

std::string Task::getKernelName() {
    return KernelName;
}

void Task::addData(std::vector<void*> value, Type type) {
    Data.emplace_back(type,value);
    IsDataSet = true;
}

std::pair<Type, std::vector<std::vector<void*>>> Task::getReturnData() {
    return std::pair<Type, std::vector<std::vector<void*>>>(ReturnDataType,ReturnData);
}

void Task::setReturnDataType(Type type) {
    ReturnDataType=type;
}

Type Task::getReturnDataType()
{
	return ReturnDataType;
}

std::vector<std::pair<Type, std::vector<void*>>> Task::getAllData() {
	if((DepType==OutsideDependancy)&&(!IsDataSet))
	{
	    for(std::function<void(void)> getExternalData : GetExternalDataMethods) {
            getExternalData();
        }

		IsDataSet=true;
	}
	if((DepType==OtherTask)&&(!IsDataSet))
	{
		readDataFromOtherThread();
		IsDataSet=true;
	}
    return Data;
}

Task Task::operator=(Task other) {
    return other;
}

DependancyType Task::dependancyType()
{
	return DepType;
}

bool Task::isCalculationDone()
{
	return IsCalculationDone;
}

std::vector<std::string> Task::kernelArguments()
{
	return KernelFileParser::getKernelArgsForKernel(Path,KernelName);
}

bool Task::hasDependencies()
{
	return DependandTasks.size() > 0;
}

bool Task::dependenciesAreCalculated()
{
	return IsDataSet;
}

void Task::setElapsedTime(float time)
{
	ElapsedTime = time;
}

float Task::elapsedTime()
{
	return ElapsedTime;
}

void Task::addConstant(Type type, void* data)
{
	ConstantData.push_back(std::pair<Type, void*>(type,data));
}

std::vector<std::pair<Type, void*>> Task::getAllConstantData()
{
	return ConstantData;
}

void Task::readDataFromOtherThread()
{
	for(Task* task : DependandTasks)
	{
	    Load = task->getLoad();
		std::vector<void*> data = task->getReturnData().second.at(0);
		Data.push_back(std::pair<Type,std::vector<void*>>(task->getReturnData().first, data));
	}
}

void Task::addReturnData(std::vector<void*> data)
{
	ReturnData.push_back(data);
	IsCalculationDone = true;
}

void Task::setDataDependancy(SCHEDULER::DependancyType type)
{
	DepType = type;
}

void Task::addDependandTask(SCHEDULER::Task* task)
{
	DependandTasks.push_back(task);
}

void Task::addExternalDataMethod(std::function<void()> externalFunctionData)
{
    GetExternalDataMethods.push_back(externalFunctionData);
}
