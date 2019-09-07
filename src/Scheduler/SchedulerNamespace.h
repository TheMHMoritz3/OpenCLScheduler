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

#ifndef UI_SCHEDULERNAMESPACE_H
#define UI_SCHEDULERNAMESPACE_H

#include "scheduler_global.h"

namespace SCHEDULER {

	enum SCHEDULER_EXPORT ScheduleType {
        SERIAL,
        STATIC,
	    ASAPHC,
        READY_FIRE_SCHEDULER
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
