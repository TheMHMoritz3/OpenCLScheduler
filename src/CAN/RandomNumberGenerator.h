#pragma once
#include <vector>

namespace CAN {
	class RandomNumberGenerator
	{
	public:
		static std::vector<void*> generateRandomNumbers(int size, int lowerBoundary = 0, int upperBoundary = 0);

	private:
	};
}
