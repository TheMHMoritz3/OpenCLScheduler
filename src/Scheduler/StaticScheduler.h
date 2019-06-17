//
// Created by moritz on 17.06.19.
//

#ifndef SCHEDULER_STATICSCHEDULER_H
#define SCHEDULER_STATICSCHEDULER_H

#include "Scheduler.h"

namespace SCHEDULER {
    class StaticScheduler : public Scheduler{
        void schedule() override;
    };
}

#endif //SCHEDULER_STATICSCHEDULER_H
