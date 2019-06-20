//
// Created by moritz on 17.06.19.
//

#ifndef EMBEDDEDSYSTEMSPROJECT_TASK_H
#define EMBEDDEDSYSTEMSPROJECT_TASK_H

#include <string>
#include <CL/cl.hpp>

namespace SCHEDULER {
    class Task {
    public:
        Task() = delete;
        Task(int id);
        int getId();
        void* getData();
        void addData(void* value);
        void setProgramSources(cl::Program::Sources sources);
        cl::Program::Sources getSources();
        void setProgam(cl::Program program);
        void setKernel(std::string kernelName);
        void addDescription(std::string desc);
    private:
        std::string Description;
        cl::Program::Sources Sources;
        cl::Program Program;
        int ID;

    };
}

#endif //EMBEDDEDSYSTEMSPROJECT_TASK_H
