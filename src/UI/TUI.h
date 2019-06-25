//
// Created by moritz on 19.06.19.
//

#ifndef UI_TUI_H
#define UI_TUI_H

#include "../Scheduler/ScheduleManager.h"
#include "../Scheduler/Task.h"
#include <string>

namespace UI {
    class TUI {
    public:
        void start();

    private:
        void decorateError(std::string errorText);
		void decorateNormalMessage(std::string message);
        void addKernelMenu();

		void setSchedule();

		void printData();
		void clear();

		void decorateValueData(SCHEDULER::Task* task);

		void decorateFloatValue(SCHEDULER::Task* task,void* data);
		void decorateIntValue(SCHEDULER::Task* task,void* data);
		void decorateUIntValue(SCHEDULER::Task* task,void* data);
		void decorateCharValue(SCHEDULER::Task* task,void* data);
		void decorateDoubleValue(SCHEDULER::Task* task,void* data);

		void askUserReturnData(SCHEDULER::Task* task);
		void decorateUnitTestingMode(SCHEDULER::Task* task);
		SCHEDULER::Type getTypeFromUserForArg();

		void askUserForArrayData(SCHEDULER::Task* task, SCHEDULER::Type type, int load);
		void* askUserForIntegerArray(int load);
		void* askUserForUIntegerArray(int load);
		void* askUserForCharArray(int load);
		void* askUserForDoubleArray(int load);
		void* askUserForFloatArray(int load);

        SCHEDULER::ScheduleManager* ScheduleManager;
		bool IsInUnitTestingMode;

		std::vector<SCHEDULER::Task*> tasks;
    };
}

#endif //UI_TUI_H
