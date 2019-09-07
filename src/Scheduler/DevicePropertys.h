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
		explicit DeviceProperties(std::string deviceName, int maxCoreCount);
		DeviceProperties() = delete;

		std::string getName();
		int getCoureCount();
		void setCoreCount(int coreCount);

		void setSchedule(ScheduleType type);
		ScheduleType getSchedule();

		void addTaskToSchedule(Task* task);
		std::vector<Task*> getTasksToSchedule();
		void clearTasksToSchedule();

		bool getOutOfOrderExecution();
		void setOutOfOrderExecution(bool ooe);

		int getMaxCoreCount();

		std::string toString();
	private:
		std::string DeviceName;
		ScheduleType Type;
		std::vector<Task*> TasksToSchedule;
		int CoreCount;
		int MaxCoreCount;
		bool OutOfOrderExecution;
	};
}

#endif //UI_DEVICEPROPERTYS_H
