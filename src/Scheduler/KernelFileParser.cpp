#include "KernelFileParser.h"
#include <fstream>
#include <iostream>

using namespace SCHEDULER;

KernelFileParser::KernelFileParser()
{
}

void KernelFileParser::parseCLFile(std::string path)
{
	std::ifstream infile(path);

	std::string line;
	for (std::string line; getline(infile, line); )
	{
		size_t found = line.find("__kernel");
		if (found != std::string::npos)
		{
			std::cout << line << std::endl;
		}
	}
}
