//
// Created by moritz on 17.06.19.
//

#include "Task.h"

using namespace SCHEDULER;


Task::Task(int id) {
    ID=id;
}

int Task::getId() {
    return ID;
}

void* Task::getData() {
    return nullptr;
}

void Task::addData(void *value) {

}

void Task::setKernel(std::string kernelName) {

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
