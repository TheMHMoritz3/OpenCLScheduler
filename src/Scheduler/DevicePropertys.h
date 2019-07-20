//
// Created by root on 20.07.19.
//

#ifndef UI_DEVICEPROPERTYS_H
#define UI_DEVICEPROPERTYS_H

#include "Task.h"
#include "SchedulerNamespace.h"
#include "scheduler_global.h"
#include <string>



namespace SCHEDULER {
	class SCHEDULER_EXPORT DeviceProperties {
	public:
		DeviceProperties(std::string deviceName);
		DeviceProperties() = delete;

		std::string getName();

		void setSchedule(ScheduleType type);
		void addTaskToSchedule(Task* task);

	private:
		std::string DeviceName;
		ScheduleType Type;
		std::vector<Task*> TasksToSchedule;
	};
}

#endif //UI_DEVICEPROPERTYS_H
