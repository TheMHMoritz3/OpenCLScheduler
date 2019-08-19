#include "RandomNumberGenerator.h"
#include <random>

using namespace UI;

std::vector<void*> RandomNumberGenerator::generateRandomNumbers(int size, SCHEDULER::Type type)
{
	switch (type)
	{
	case SCHEDULER::INT:
		return generateUIntValues(size);
	case SCHEDULER::UINT:
		return generateIntValues(size);
	case SCHEDULER::CHAR:
		return generateCharValues(size);
	case SCHEDULER::FLOAT:
		return generateFloatValues(size);
	case SCHEDULER::DOUBLE:
		return generateDoubleValues(size);
	case SCHEDULER::STRING:
		return generateStringValues(size);
	default: 
		break;
	}
}

std::vector<void*> RandomNumberGenerator::generateUIntValues(int size)
{
	std::random_device rd;
	std::mt19937_64 gen(rd());

	/* This is where you define the number generator for unsigned long long: */
	std::uniform_int_distribution<unsigned int> dis;
	std::vector<void*> returnValue;
	uint32_t* value = new uint32_t[size];
	for(int i = 0; i<size; i++)
	{
		value[i] = dis(gen);
		returnValue.push_back(static_cast<void*>(&value[i]));
	}
	return returnValue;
}

std::vector<void*> RandomNumberGenerator::generateIntValues(int size)
{
	std::random_device rd;
	std::mt19937_64 gen(rd());

	/* This is where you define the number generator for unsigned long long: */
	std::uniform_int_distribution<int> dis;
	std::vector<void*> returnValue;
	int32_t* value = new int32_t[size];
	for (int i = 0; i < size; i++)
	{
		value[i] = dis(gen);
		returnValue.push_back(static_cast<void*>(&value[i]));
	}
	return returnValue;
}

std::vector<void*> RandomNumberGenerator::generateFloatValues(int size)
{
	std::random_device rd;
	std::mt19937_64 gen(rd());

	/* This is where you define the number generator for unsigned long long: */
	std::uniform_int_distribution<int> dis;
	std::vector<void*> returnValue;
	float* value = new float[size];
	for (int i = 0; i < size; i++)
	{
		value[i] = dis(gen);
		returnValue.push_back(static_cast<void*>(&value[i]));
	}
	return returnValue;
}

std::vector<void*> RandomNumberGenerator::generateCharValues(int size)
{
	std::random_device rd;
	std::mt19937_64 gen(rd());

	/* This is where you define the number generator for unsigned long long: */
	std::uniform_int_distribution<int> dis;
	std::vector<void*> returnValue;
	char* value = new char[size];
	for (int i = 0; i < size; i++)
	{
		value[i] = dis(gen);
		returnValue.push_back(static_cast<void*>(&value[i]));
	}
	return returnValue;
}

std::vector<void*> RandomNumberGenerator::generateDoubleValues(int size)
{
	std::random_device rd;
	std::mt19937_64 gen(rd());

	/* This is where you define the number generator for unsigned long long: */
	std::uniform_int_distribution<int> dis;
	std::vector<void*> returnValue;
	double* value = new double[size];
	for (int i = 0; i < size; i++)
	{
		value[i] = dis(gen);
		returnValue.push_back(static_cast<void*>(&value[i]));
	}
	return returnValue;
}

std::vector<void*> RandomNumberGenerator::generateStringValues(int size)
{
	std::random_device rd;
	std::mt19937_64 gen(rd());

	/* This is where you define the number generator for unsigned long long: */
	std::uniform_int_distribution<int> dis;
	std::vector<void*> returnValue;
	char* value = new char[size];
	for (int i = 0; i < size; i++)
	{
		value[i] = dis(gen);
		returnValue.push_back(static_cast<void*>(&value[i]));
	}
	return returnValue;
}
