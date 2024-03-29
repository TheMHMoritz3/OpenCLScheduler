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

#ifndef EMBEDDEDSYSTEMSPROJECT_TASK_H
#define EMBEDDEDSYSTEMSPROJECT_TASK_H

#include <utility>
#include <vector>
#include <string>
#include <functional>
#include <CL/cl.hpp>
#include "SchedulerNamespace.h"
#include "scheduler_global.h"

namespace SCHEDULER {
	class SCHEDULER_EXPORT Task {
    public:
        Task(int id);
        Task() = delete;
		void setProgam(cl::Program program);
		void setKernel(std::string kernelName);
		void setPath(std::string path);
		void setLoad(int load);
		void setReadBuffer(cl::Buffer* readBuffer);
		void setProgramSources(cl::Program::Sources *sources);
		void addData(std::vector<void*> value, Type type);
		void addDescription(std::string desc);
		void setReturnDataType(Type type);
		void addReturnData(std::vector<void*> data);
		void setDataDependancy(SCHEDULER::DependancyType type);
		void addDependandTask(SCHEDULER::Task* task);
		void addExternalDataMethod(std::function<void(void)> externalFunctionData);

        int getId();
        std::pair<Type, std::vector<std::vector<void*>>> getReturnData();
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
		std::vector<std::string> kernelArguments();
		bool hasDependencies();
		bool dependenciesAreCalculated();

		void addElapsedTime(unsigned long time);
		std::vector<unsigned long> elapsedTime();

		void addConstant(Type type, void* data);
		std::vector<std::pair<Type, void*>> getAllConstantData();

		std::vector<std::string> getTaskConstants();

		std::vector<Task*> getDependantTasks();

		bool isScheduled();
		void setScheduled(bool scheduled);

        bool isCalculationDone();
        void resetIsCalculationDone();
    private:
	    void addINTConstant(void* data);
	    void addUINTConstant(void* data);
	    void addDOUBLEConstant(void* data);
	    void addCHARConstant(void* data);
	    void addFloatConstant(void* data);
		void readDataFromOtherThread();

        std::string KernelName;
		std::string Path;
        std::string Description;
        cl::Program::Sources *Sources;
        cl::Program Program;
		cl::Buffer *ReadBuffer;
        int ID;
		int Load;
        std::vector<std::pair<Type, std::vector<void*>>> Data;
        std::vector<std::vector<void*>> ReturnData;
		std::vector<std::pair<Type,void*>> ConstantData;
        Type ReturnDataType;
        std::vector<Task*> DependandTasks;
		DependancyType DepType;
		bool IsCalculationDone;
		bool IsDataSet;
		std::vector<std::function<void(void)>> GetExternalDataMethods;
		std::vector<unsigned long> ElapsedTime;
		bool IsScheduled;
    };
}

#endif //EMBEDDEDSYSTEMSPROJECT_TASK_H
