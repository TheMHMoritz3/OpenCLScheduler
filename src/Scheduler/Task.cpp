//
// Created by moritz on 17.06.19.
//

#include "Task.h"
#include <iostream>

using namespace SCHEDULER;


Task::Task(int id) {
    ID=id;
    ReturnData=NULL;
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
    //std::cout<<"Task::setProgrammSources"<<sources->at(0).first<<std::endl;
    Sources=sources;
}

void Task::setProgam(cl::Program program) {
    Program=program;
}

cl::Program::Sources* Task::getSources() {
    return Sources;
}

void Task::setReadBuffer(cl::Buffer readBuffer)
{
	ReadBuffer = readBuffer;
}

cl::Buffer SCHEDULER::Task::Task::readBuffer()
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

void Task::addData(void *value, Type type) {
    Data.emplace_back(type,value);
}

std::pair<Type, void *> Task::getReturnData() {
    return std::pair<Type, void*>(ReturnDataType,ReturnData);
}

void Task::setReturnDataType(Type type) {
    ReturnDataType=type;
}

Type Task::getReturnDataType()
{
	return ReturnDataType;
}

std::vector<std::pair<Type, void *>> Task::getAllData() {
    return Data;
}

Task Task::operator=(Task other) {
    return other;
}

void Task::setReturnData(void* data)
{
	ReturnData = data;
}

void Task::setFilePath(std::string filePath) {
    FilePath=filePath;
}

std::string Task::getFilePath() {
    return FilePath;
}
