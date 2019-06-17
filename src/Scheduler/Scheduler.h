//
// Created by moritz on 17.06.19.
//

#ifndef EMBEDDEDSYSTEMSPROJECT_SCHEDULER_H
#define EMBEDDEDSYSTEMSPROJECT_SCHEDULER_H

#include <vector>
#include "Task.h"

namespace SCHEDULER {
    class Scheduler {
    public:
        Scheduler(std::vector<Task> Tasks);
        virtual void schedule()=0;

    protected:
        std::vector<Task> Tasks;

    private:
        Scheduler() = delete;
    };
}


#endif //EMBEDDEDSYSTEMSPROJECT_SCHEDULER_H
