//
// Created by moritz on 17.06.19.
//

#ifndef EMBEDDEDSYSTEMSPROJECT_TASK_H
#define EMBEDDEDSYSTEMSPROJECT_TASK_H

#include <vector>
#include <string>
#include <CL/cl.hpp>
#include "SchedulerNamespace.h"

namespace SCHEDULER {
    class Task {
    public:
        Task() = delete;
        Task(int id);
        int getId();
        std::pair<Type,void*> getReturnData();
        void addData(void* value, Type type);
        std::vector<std::pair<Type, void*>> getAllData();
        void setProgramSources(cl::Program::Sources sources);
        cl::Program::Sources getSources();
        void setProgam(cl::Program program);
        void setKernel(std::string kernelName);
        void addDescription(std::string desc);
        std::string getKernelName();
        std::string getDescription();
        void setReturnDataType(Type type);
        Task operator=(Task other);

    private:
        std::string KernelName;
        std::string Description;
        cl::Program::Sources Sources;
        cl::Program Program;
        int ID;
        std::vector<std::pair<Type, void*>> Data;
        void* ReturnData;
        Type ReturnDataType;
    };
}

#endif //EMBEDDEDSYSTEMSPROJECT_TASK_H
