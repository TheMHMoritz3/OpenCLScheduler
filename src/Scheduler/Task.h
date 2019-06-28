//
// Created by moritz on 17.06.19.
//

#ifndef EMBEDDEDSYSTEMSPROJECT_TASK_H
#define EMBEDDEDSYSTEMSPROJECT_TASK_H

#include <vector>
#include <string>
#include <CL/cl.hpp>
#include "SchedulerNamespace.h"
#include "scheduler_global.h"

namespace SCHEDULER {
	class SCHEDULER_EXPORT Task {
    public:
        Task() = delete;
        Task(int id);

		void setProgam(cl::Program program);
		void setKernel(std::string kernelName);
		void setLoad(int load);
		void setReadBuffer(cl::Buffer* readBuffer);
		void setProgramSources(cl::Program::Sources *sources);
		void addData(std::vector<void*> value, Type type);
		void addDescription(std::string desc);
		void setReturnDataType(Type type);
		void setReturnData(std::vector<void*> data);
		void addUINTItems(uint32_t* items);

        int getId();
        std::pair<Type,std::vector<void*>> getReturnData();
        std::vector<std::pair<Type, std::vector<void*>>> getAllData();
        cl::Program::Sources *getSources();
		cl::Buffer *readBuffer();
		cl::Program getProgramm();
		int getLoad();
        std::string getKernelName();
        std::string getDescription();
		Type getReturnDataType();
		Task operator=(Task other);

    private:
        std::string KernelName;
        std::string Description;
        cl::Program::Sources *Sources;
        cl::Program Program;
		cl::Buffer *ReadBuffer;
        int ID;
		int Load;
        std::vector<std::pair<Type, std::vector<void*>>> Data;
        std::vector<void*> ReturnData;
        Type ReturnDataType;
        std::vector<Task*> DependandTask;
    };
}

#endif //EMBEDDEDSYSTEMSPROJECT_TASK_H
