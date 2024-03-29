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
	IsScheduled = false;
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
	bool dependanciesAreCalculated = true;
	if(hasDependencies())
	{
		for(Task* task : DependandTasks)
		{
			if(dependanciesAreCalculated)
				dependanciesAreCalculated = task->isCalculationDone();
		}
	}
	return dependanciesAreCalculated;
}

void Task::addElapsedTime(unsigned long time)
{
	ElapsedTime.push_back(time);
}

std::vector<unsigned long> Task::elapsedTime()
{
	return ElapsedTime;
}

void Task::addConstant(Type type, void* data)
{
    switch (type){
        case INT:
            addINTConstant(data);
            break;
        case UINT:
            addUINTConstant(data);
            break;
        case CHAR:
            addCHARConstant(data);
            break;
        case FLOAT:
            addFloatConstant(data);
            break;
        case DOUBLE:
            addDOUBLEConstant(data);
            break;
        case STRING:
        default:
            break;
    }
    IsDataSet=false;
//	ConstantData.push_back(std::pair<Type, void*>(type,data));
}

std::vector<std::pair<Type, void*>> Task::getAllConstantData()
{
	return ConstantData;
}

std::vector<std::string> Task::getTaskConstants()
{
	return KernelFileParser::getKernelConstantsForKernel(Path, KernelName);
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

std::vector<Task *> Task::getDependantTasks() {
    return DependandTasks;
}

void Task::addINTConstant(void *data) {
    std::vector<void*> dataSet;
    int* value = new int[getLoad()];
    for(int i = 0; i<getLoad(); i++){
        value[i]=*((int*)data);
        dataSet.push_back(&value[i]);
    }
    addData(dataSet,INT);
}

void Task::addUINTConstant(void *data) {
    std::vector<void*> dataSet;
    unsigned int* value = new unsigned int[getLoad()];
    for(int i = 0; i<getLoad(); i++){
        value[i]=*((unsigned int*)data);
        dataSet.push_back(&value[i]);
    }
    addData(dataSet,UINT);
}

void Task::addDOUBLEConstant(void *data) {
    std::vector<void*> dataSet;
    double* value = new double[getLoad()];
    for(int i = 0; i<getLoad(); i++){
        value[i]=*((double*)data);
        dataSet.push_back(&value[i]);
    }
    addData(dataSet,DOUBLE);
}

void Task::addCHARConstant(void *data) {
    std::vector<void*> dataSet;
    char* value = new char[getLoad()];
    for(int i = 0; i<getLoad(); i++){
        value[i]=*((char*)data);
        dataSet.push_back(&value[i]);
    }
    addData(dataSet,CHAR);
}

void Task::addFloatConstant(void *data) {
    std::vector<void*> dataSet;
    float* value = new float[getLoad()];
    for(int i = 0; i<getLoad(); i++){
        value[i]=*((float*)data);
        dataSet.push_back(&value[i]);
    }
    addData(dataSet,FLOAT);
}

void Task::setScheduled(bool scheduled) {
    IsScheduled = scheduled;
}

bool Task::isScheduled() {
    return IsScheduled;
}

void Task::resetIsCalculationDone() {
    IsCalculationDone = false;
}
