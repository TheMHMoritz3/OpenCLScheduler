//
// Created by moritz on 19.06.19.
//

#ifndef UI_TUI_H
#define UI_TUI_H

#include "../Scheduler/ScheduleManager.h"
#include "../Scheduler/Task.h"
#include <string>
#include <vector>

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

		void decorateFloatValue(SCHEDULER::Task* task,std::vector<void*> data);
		void decorateIntValue(SCHEDULER::Task* task, std::vector<void*> data);
		void decorateUIntValue(SCHEDULER::Task* task, std::vector<void*> data);
		void decorateCharValue(SCHEDULER::Task* task, std::vector<void*> data);
		void decorateDoubleValue(SCHEDULER::Task* task, std::vector<void*> data);

		void askUserReturnData(SCHEDULER::Task* task);
		void decorateUnitTestingMode(SCHEDULER::Task* task);
		SCHEDULER::Type getTypeFromUserForArg();

		void askUserForArrayData(SCHEDULER::Task* task, SCHEDULER::Type type, int load);
		std::vector<void*> askUserForIntegerArray(int load);
		std::vector<void*> askUserForUIntegerArray(int load);
		std::vector<void*> askUserForCharArray(int load);
		std::vector<void*> askUserForDoubleArray(int load);
		std::vector<void*> askUserForFloatArray(int load);

        SCHEDULER::ScheduleManager* ScheduleManager;
		bool IsInUnitTestingMode;

		std::vector<SCHEDULER::Task*> tasks;
    };
}

#endif //UI_TUI_H
