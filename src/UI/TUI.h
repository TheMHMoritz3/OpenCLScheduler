//
// Created by moritz on 19.06.19.
//

#ifndef UI_TUI_H
#define UI_TUI_H

#include "../Scheduler/ScheduleManager.h"

#include <string>

namespace UI {
    class TUI {
    public:
        void start();

    private:
        void decorateError(std::string errorText);
        void addKernelMenu();

        SCHEDULER::ScheduleManager* ScheduleManager;
    };
}

#endif //UI_TUI_H
