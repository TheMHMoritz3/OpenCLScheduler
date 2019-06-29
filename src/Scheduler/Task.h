//
// Created by moritz on 17.06.19.
//

#ifndef EMBEDDEDSYSTEMSPROJECT_TASK_H
#define EMBEDDEDSYSTEMSPROJECT_TASK_H

#include <vector>
#include <string>
#include <functional>
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
		void setDataDependancy(SCHEDULER::DependancyType type);
		void addDependandTask(SCHEDULER::Task* task);
		void setExternalDataMethod(std::function<void(void)> externalFunctionData);

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
		DependancyType dependancyType();
		bool isCalculationDone();

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
        std::vector<Task*> DependandTasks;
		DependancyType DepType;
		bool IsCalculationDone;
		std::function<void(void)> GetExternalData;

    };
}

#endif //EMBEDDEDSYSTEMSPROJECT_TASK_H
