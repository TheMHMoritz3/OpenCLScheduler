//
// Created by moritz on 17.06.19.
//

#include "Task.h"
#include <iostream>

using namespace SCHEDULER;


Task::Task(int id) {
    ID=id;
	IsCalculationDone = false;
}

int Task::getId() {
    return ID;
}

void Task::setKernel(std::string kernelName) {
    KernelName = kernelName;
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
}

std::pair<Type, std::vector<void *>> Task::getReturnData() {
    return std::pair<Type, std::vector<void*>>(ReturnDataType,ReturnData);
}

void Task::setReturnDataType(Type type) {
    ReturnDataType=type;
}

Type Task::getReturnDataType()
{
	return ReturnDataType;
}

std::vector<std::pair<Type, std::vector<void*>>> Task::getAllData() {
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

void Task::setReturnData(std::vector<void*> data)
{
	ReturnData = data;
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
