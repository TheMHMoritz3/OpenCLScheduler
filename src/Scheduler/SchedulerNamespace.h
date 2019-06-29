//
// Created by moritz on 17.06.19.
//

#ifndef UI_SCHEDULERNAMESPACE_H
#define UI_SCHEDULERNAMESPACE_H

#include "scheduler_global.h"

namespace SCHEDULER {

	enum SCHEDULER_EXPORT ScheduleType {
        LIST,
        ASAPHC,
        STATIC
    };

	enum SCHEDULER_EXPORT ErrorCodes {
        COMPILATION_NOT_POSSIBLE
    };

	enum SCHEDULER_EXPORT Type {
        INT,
        UINT,
        CHAR,
        FLOAT,
        DOUBLE,
        STRING
    };

	enum SCHEDULER_EXPORT DependancyType
	{
		UserInput,
		OtherTask,
		OutsideDependancy
	};
}

#endif //UI_SCHEDULERNAMESPACE_H
