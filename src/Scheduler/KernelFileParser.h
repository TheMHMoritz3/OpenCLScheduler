#pragma once

#include <string>
#include <vector>
#include "Task.h"


namespace SCHEDULER {
	class KernelFileParser
	{
	public:
		static std::vector<std::string> getKernelNames(std::string path);
		static std::vector<std::vector<std::string>> getKernelArgs(std::string path);
		static std::vector<std::string> getKernelArgsForKernel(std::string path, std::string kernelName);
	};
}
