#pragma once
#include <vector>
#include "src/Scheduler/SchedulerNamespace.h"

namespace UI {
	class RandomNumberGenerator
	{
	public:
		static std::vector<void*> generateRandomNumbers(int size, SCHEDULER::Type type);

	private:
		static std::vector<void*> generateUIntValues(int size);
		static std::vector<void*> generateIntValues(int size);
		static std::vector<void*> generateFloatValues(int size);
		static std::vector<void*> generateCharValues(int size);
		static std::vector<void*> generateDoubleValues(int size);
		static std::vector<void*> generateStringValues(int size);

	};
}
