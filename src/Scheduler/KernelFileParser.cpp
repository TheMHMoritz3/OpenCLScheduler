#include "KernelFileParser.h"
#include <fstream>
#include <iostream>

using namespace SCHEDULER;

//
//void KernelFileParser::parseCLFile(std::string path)
//{
//	std::ifstream infile(path);
//
//	std::string line;
//	for (std::string line; getline(infile, line); )
//	{
//		std::vector<std::string> container;
//		size_t found = line.find("__kernel");
//		if (found != std::string::npos)
//		{
//			container.clear();
//			size_t len = line.length() + 1;
//			char* charArray = new char[len];
//			memset(charArray, 0, len * sizeof(char));
//			memcpy(charArray, line.c_str(), (len - 1) * sizeof(char));
//			for (char* p = strtok(charArray, "("); p != NULL; p = strtok(NULL, "("))
//			{
//				container.push_back(p);
//			}
//			delete[] charArray;
//
//			std::string kernelStartString = container.at(0);
//
//			std::vector<std::string> kernelStart;
//			size_t length = kernelStartString.length() + 1;
//			charArray = new char[length];
//			memset(charArray, 0, length * sizeof(char));
//			memcpy(charArray, kernelStartString.c_str(), (length - 1) * sizeof(char));
//			for (char* p = strtok(charArray, " "); p != NULL; p = strtok(NULL, " "))
//			{
//				kernelStart.push_back(p);
//			}
//			std::cout << kernelStart.at(2)<< std::endl;
//			delete[] charArray;
//
//			std::string kernelEndString = container.at(1);
//
//			std::vector<std::string> kernelArgs;
//			size_t length2 = kernelEndString.length() + 1;
//			charArray = new char[length2];
//			memset(charArray, 0, length2 * sizeof(char));
//			memcpy(charArray, kernelEndString.c_str(), (length2 - 1) * sizeof(char));
//			for (char* p = strtok(charArray, ","); p != NULL; p = strtok(NULL, ","))
//			{
//				std::string arg = p;
//				if((arg.find("const")!=std::string::npos)&& (arg.find("WLOAD") == std::string::npos))
//				{
//					std::cout << p << std::endl;
//					kernelArgs.push_back(p);
//				}
//			}
//			
//			delete[] charArray;
//		}
//	}
//}

std::vector<std::string> KernelFileParser::getKernelNames(std::string path)
{
	std::vector<std::string> returnValue;
	std::ifstream infile(path);

	std::string line;
	for (std::string line; getline(infile, line); )
	{
		std::vector<std::string> container;
		size_t found = line.find("__kernel");
		if (found != std::string::npos)
		{
			container.clear();
			size_t len = line.length() + 1;
			char* charArray = new char[len];
			memset(charArray, 0, len * sizeof(char));
			memcpy(charArray, line.c_str(), (len - 1) * sizeof(char));
			for (char* p = strtok(charArray, "("); p != NULL; p = strtok(NULL, "("))
			{
				container.push_back(p);
			}
			delete[] charArray;

			std::string kernelStartString = container.at(0);

			std::vector<std::string> kernelStart;
			size_t length = kernelStartString.length() + 1;
			charArray = new char[length];
			memset(charArray, 0, length * sizeof(char));
			memcpy(charArray, kernelStartString.c_str(), (length - 1) * sizeof(char));
			for (char* p = strtok(charArray, " "); p != NULL; p = strtok(NULL, " "))
			{
				kernelStart.push_back(p);
			}
			returnValue.push_back(kernelStart.at(2));
			delete[] charArray;
		}
		returnValue = container;
	}
	return returnValue;
}

std::vector<std::vector<std::string>> KernelFileParser::getKernelArgs(std::string path)
{
	std::vector<std::vector<std::string>> returnValue;

	std::ifstream infile(path);

	std::string line;
	for (std::string line; getline(infile, line); )
	{

		std::vector<std::string> container;
		size_t found = line.find("__kernel");
		if (found != std::string::npos)
		{
			container.clear();
			size_t len = line.length() + 1;
			char* charArray = new char[len];
			memset(charArray, 0, len * sizeof(char));
			memcpy(charArray, line.c_str(), (len - 1) * sizeof(char));
			for (char* p = strtok(charArray, "("); p != NULL; p = strtok(NULL, "("))
			{
				container.push_back(p);
			}
			delete[] charArray;

			std::string kernelEndString = container.at(1);

			std::vector<std::string> kernelArgs;
			size_t length2 = kernelEndString.length() + 1;
			charArray = new char[length2];
			memset(charArray, 0, length2 * sizeof(char));
			memcpy(charArray, kernelEndString.c_str(), (length2 - 1) * sizeof(char));
			for (char* p = strtok(charArray, ","); p != NULL; p = strtok(NULL, ","))
			{
				std::string arg = p;
				if ((arg.find("const") != std::string::npos) && (arg.find("WLOAD") == std::string::npos))
				{
					kernelArgs.push_back(p);
				}
			}
			returnValue.push_back(kernelArgs);
			delete[] charArray;
		}
	}
	return returnValue;
}

std::vector<std::string> KernelFileParser::getKernelArgsForKernel(std::string path, std::string kernelName)
{
	std::vector<std::string> returnValue;

	std::ifstream infile(path);
	std::string line;
	for (std::string line; getline(infile, line); )
	{

		std::vector<std::string> container;
		size_t found = line.find("__kernel");
		if ((found != std::string::npos) && (line.find(kernelName) != std::string::npos))
		{

			container.clear();
			size_t len = line.length() + 1;
			char* charArray = new char[len];
			memset(charArray, 0, len * sizeof(char));
			memcpy(charArray, line.c_str(), (len - 1) * sizeof(char));
			for (char* p = strtok(charArray, "("); p != NULL; p = strtok(NULL, "("))
			{
				container.push_back(p);
			}
			delete[] charArray;
			std::string kernelEndString = container.at(1);

			std::vector<std::string> kernelArgs;
			size_t length2 = kernelEndString.length() + 1;
			charArray = new char[length2];
			memset(charArray, 0, length2 * sizeof(char));
			memcpy(charArray, kernelEndString.c_str(), (length2 - 1) * sizeof(char));
			for (char* p = strtok(charArray, ","); p != NULL; p = strtok(NULL, ","))
			{
				std::string arg = p;
				bool isWloadInside = ((arg.find("WLOAD") != std::string::npos) || (arg.find("wLoad") != std::string::npos));
				if ((arg.find("const") != std::string::npos) && (arg.find("*") != std::string::npos) && !isWloadInside)
				{
					kernelArgs.push_back(p);
				}
			}
			returnValue = kernelArgs;
			delete[] charArray;
		}
	}
	return returnValue;
}

std::vector<std::string> KernelFileParser::getKernelConstantsForKernel(std::string path, std::string kernelName)
{
	std::vector<std::string> returnValue;

	std::ifstream infile(path);
	std::string line;
	for (std::string line; getline(infile, line); )
	{

		std::vector<std::string> container;
		size_t found = line.find("__kernel");
		if ((found != std::string::npos) && (line.find(kernelName) != std::string::npos))
		{

			container.clear();
			size_t len = line.length() + 1;
			char* charArray = new char[len];
			memset(charArray, 0, len * sizeof(char));
			memcpy(charArray, line.c_str(), (len - 1) * sizeof(char));
			for (char* p = strtok(charArray, "("); p != NULL; p = strtok(NULL, "("))
			{
				container.push_back(p);
			}
			delete[] charArray;
			std::string kernelEndString = container.at(1);

			std::vector<std::string> kernelArgs;
			size_t length2 = kernelEndString.length() + 1;
			charArray = new char[length2];
			memset(charArray, 0, length2 * sizeof(char));
			memcpy(charArray, kernelEndString.c_str(), (length2 - 1) * sizeof(char));
			for (char* p = strtok(charArray, ","); p != NULL; p = strtok(NULL, ","))
			{
				std::string arg = p;
				bool isWloadInside = ((arg.find("WLOAD") != std::string::npos) || (arg.find("wLoad") != std::string::npos));
				if ((arg.find("const") != std::string::npos) && (arg.find("*") == std::string::npos) && !isWloadInside)
				{
					kernelArgs.push_back(p);
				}
			}
			returnValue = kernelArgs;
			delete[] charArray;
		}
	}
	return returnValue;
}
