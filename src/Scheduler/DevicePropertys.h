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
		int getCoureCount();
		void setCoreCount(int coreCount);

		void setSchedule(ScheduleType type);
		ScheduleType getSchedule();
		void addTaskToSchedule(Task* task);
		std::vector<Task*> getTasksToSchedule();


	private:
		std::string DeviceName;
		ScheduleType Type;
		std::vector<Task*> TasksToSchedule;
		int CoreCount;
	};
}

#endif //UI_DEVICEPROPERTYS_H
