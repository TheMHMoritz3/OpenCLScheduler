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

#ifndef UI_TUI_H
#define UI_TUI_H

#include "../Scheduler/ScheduleManager.h"
#include "../Scheduler/Task.h"
#include "../CAN/CanManager.h"
#include <string>
#include <vector>

namespace UI {
    class TUI {
    public:
		TUI();

        void start();

        void toggleUnitTestMode();
    private:
        void decorateError(std::string errorText);
		void decorateNormalMessage(std::string message);

		void addKernelMenu();
        void applyStaticMode();


		void setSchedule();

		void decorateNormalMode(SCHEDULER::Task* task);

		void activateCanBus();

		void printData();
		void clear();

		void decorateValueData(SCHEDULER::Task* task);

		void decorateFloatValue(SCHEDULER::Task* task,std::vector<void*> data, int load);
		void decorateIntValue(SCHEDULER::Task* task, std::vector<void*> data, int load);
		void decorateUIntValue(SCHEDULER::Task* task, std::vector<void*> data, int load);
		void decorateCharValue(SCHEDULER::Task* task, std::vector<void*> data, int load);
		void decorateDoubleValue(SCHEDULER::Task* task, std::vector<void*> data, int load);

		void decorateOtherTask(SCHEDULER::Task* task);
		void decorateCan(SCHEDULER::Task* task, int load);
		void askUserReturnData(SCHEDULER::Task* task);
		void decorateUnitTestingMode(SCHEDULER::Task* task);
		SCHEDULER::Type getTypeFromUserForArg();

		void askUserForArrayData(SCHEDULER::Task* task, SCHEDULER::Type type, int load);
		std::vector<void*> askUserForIntegerArray(int load);
		std::vector<void*> askUserForUIntegerArray(int load, SCHEDULER::Task* task);
		std::vector<void*> askUserForCharArray(int load);
		std::vector<void*> askUserForDoubleArray(int load);
		std::vector<void*> askUserForFloatArray(int load);

        SCHEDULER::ScheduleManager* ScheduleManager;
        CAN::CanManager* CanManager;

		bool IsInUnitTestingMode;

		std::vector<SCHEDULER::Task*> tasks;
    };
}

#endif //UI_TUI_H
