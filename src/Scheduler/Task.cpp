//
// Created by moritz on 17.06.19.
//

#include "Task.h"

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

void Task::addDescription(std::string desc) {
    Description=desc;
}

void Task::setProgramSources(cl::Program::Sources sources) {
    Sources=sources;
}

void Task::setProgam(cl::Program program) {
    Program=program;
}

cl::Program::Sources Task::getSources() {
    return Sources;
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

std::vector<std::pair<Type, void *>> Task::getAllData() {
    return Data;
}

Task Task::operator=(Task other) {
    return other;
}
