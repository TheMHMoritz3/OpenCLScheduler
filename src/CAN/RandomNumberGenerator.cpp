#include "RandomNumberGenerator.h"
#include <random>

using namespace CAN;

std::vector<void*> RandomNumberGenerator::generateRandomNumbers(int size, int lowerBoundary, int upperBoundary)
{
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<unsigned int> dis;

	if (!((lowerBoundary == 0) && (upperBoundary == 0)))
	{
		dis = std::uniform_int_distribution<unsigned int>(lowerBoundary, upperBoundary);
	}
	std::vector<void*> returnValue;
	uint32_t* value = new uint32_t[size];
	for (int i = 0; i < size; i++)
	{
		value[i] = dis(gen);
		returnValue.push_back(static_cast<void*>(&value[i]));
	}
	return returnValue;
}