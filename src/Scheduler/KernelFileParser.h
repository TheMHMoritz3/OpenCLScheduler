#pragma once

#include "scheduler_global.h"
#include <string>
#include <vector>
#include "Task.h"


namespace SCHEDULER {
	class SCHEDULER_EXPORT KernelFileParser
	{
	public:
		KernelFileParser();

		void parseCLFile(std::string path);
	private:
		std::vector<Task*> Tasks;
	};
}
