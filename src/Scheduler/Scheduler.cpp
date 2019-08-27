/*
Embedded Systems Project 2019
Copyright (C) 2019  Moritz Herzog, Philip Lersch

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>


#include <iostream>
#include "Scheduler.h"

using namespace SCHEDULER;

Scheduler::Scheduler(std::vector<Task*> tasks, std::vector<Device*> devices) {
	Tasks = tasks;
	Devices = devices;
	CoreCount = 1;
}

void Scheduler::setRAMForCurrentTask(Task* task, Device* device, cl::Kernel kernel, cl::CommandQueue queue) {
	int count = 0;
	for (std::pair<Type, std::vector<void*>> value : task->getAllData()) {
		cl::Buffer* buffer;
		switch (value.first) {
		case Type::UINT:
			buffer = generateBufferForUINT(value.second, device->getDeviceContext(), queue, 0);
			break;
		case Type::INT:
			buffer = generateBufferForINT(value.second, device->getDeviceContext(), queue, 0);
			break;
		case Type::CHAR:
			buffer = generateBufferForCHAR(value.second, device->getDeviceContext(), queue, 0);
			break;
		case Type::DOUBLE:
			buffer = generateBufferForDOUBLE(value.second, device->getDeviceContext(), queue, 0);
			break;
		case Type::FLOAT:
			buffer = generateBufferForFLOAT(value.second, device->getDeviceContext(), queue, 0);
			break;
		case Type::STRING:
			buffer = generateBufferForCHAR(value.second, device->getDeviceContext(), queue, 0);
			break;
		default:
			break;
		}
		ErrorCode = kernel.setArg(count, *buffer);
		std::cout << "Set Ram for Current Task: "<<ErrorCode<<std::endl;
		std::cout << "Count "<<count<<std::endl;
		count++;
	}
}

void Scheduler::setRAMBufferForOutput(Task* task, Device* device, cl::Kernel kernel)
{
	cl::Buffer* buffer;
	switch (task->getReturnDataType())
	{
	case Type::UINT:
		buffer = new cl::Buffer(device->getDeviceContext(), CL_MEM_READ_WRITE, sizeof(uint32_t) * task->getLoad());
		break;
	case Type::INT:
		buffer = new cl::Buffer(device->getDeviceContext(), CL_MEM_READ_WRITE, sizeof(int32_t) * task->getLoad());
		break;
	case Type::CHAR:
		buffer = new cl::Buffer(device->getDeviceContext(), CL_MEM_READ_WRITE, sizeof(char) * task->getLoad());
		break;
	case Type::DOUBLE:
		buffer = new cl::Buffer(device->getDeviceContext(), CL_MEM_READ_WRITE, sizeof(double) * task->getLoad());
		break;
	case Type::FLOAT:
		buffer = new cl::Buffer(device->getDeviceContext(), CL_MEM_READ_WRITE, sizeof(float) * task->getLoad());
		break;
	case Type::STRING:
		buffer = new cl::Buffer(device->getDeviceContext(), CL_MEM_READ_WRITE, sizeof(char) * task->getLoad());
		break;
	default:
		break;
	}
	task->setReadBuffer(buffer);
	ErrorCode = kernel.setArg(task->getAllData().size() + task->getAllConstantData().size(), *buffer);
	std::cout << "Set Ram Output for Current Task: "<<ErrorCode<<std::endl;
	std::cout << "Count "<<task->getAllData().size()<<std::endl;
}

void Scheduler::setKernelLoad(Task* task, Device* device, cl::Kernel kernel)
{
	int ErrorCode = 0;
	cl::Buffer* buffer_WORKLOAD = new cl::Buffer(device->getDeviceContext(), CL_MEM_READ_WRITE, sizeof(int), &ErrorCode);
	ErrorCode = kernel.setArg(task->getAllData().size() + task->getAllConstantData().size() + 1, task->getLoad() / CoreCount);
	std::cout << "Kernel Load Error Code: " << ErrorCode;
}

void Scheduler::enqueueTak(Task* task, Device* device, cl::CommandQueue commandQueue, cl::Kernel kernel)
{
	ErrorCode = commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(CoreCount), cl::NDRange(CoreCount));
	std::cout << "Enqueue Task: " << ErrorCode << std::endl;
}

void Scheduler::readDataFromTask(Task* task, cl::CommandQueue commandQueue)
{
	std::vector<void*> data;
	switch (task->getReturnDataType())
	{
	case Type::UINT:
		data = readDataFromBufferForUINT(task, commandQueue, 0);
		break;
	case Type::INT:
		data = readDataFromBufferForINT(task, commandQueue, 0);
		break;
	case Type::CHAR:
		data = readDataFromBufferForCHAR(task, commandQueue, 0);
		break;
	case Type::DOUBLE:
		data = readDataFromBufferForDOUBLE(task, commandQueue, 0);
		break;
	case Type::FLOAT:
		data = readDataFromBufferForFLOAT(task, commandQueue, 0);
		break;
	case Type::STRING:
		data = readDataFromBufferForCHAR(task, commandQueue, 0);
		break;
	default:
		break;
	}
	std::cout<<"Read Data from Task ErrorCode: "<<ErrorCode;
	task->addReturnData(data);
}

void Scheduler::readConstantsFromTask(Task* task, Device* device, cl::Kernel kernel, cl::CommandQueue commandQueue)
{
	int i = task->getAllData().size();
	for (std::pair<Type, void*> value : task->getAllConstantData())
	{
		cl::Buffer* bufferToAdd;
		switch (value.first) {
		case INT:
			bufferToAdd = generateBufferForINTConstant(value.second, device->getDeviceContext(), commandQueue, i,kernel);
			break;
		case UINT:
			bufferToAdd = generateBufferForUINTConstant(value.second, device->getDeviceContext(), commandQueue, i,kernel);
			break;
		case CHAR:
			bufferToAdd = generateBufferForCHARConstant(value.second, device->getDeviceContext(), commandQueue, i,kernel);
			break;
		case FLOAT:
			bufferToAdd = generateBufferForFLOATConstant(value.second, device->getDeviceContext(), commandQueue, i,kernel);
			break;
		case DOUBLE:
			bufferToAdd = generateBufferForDOUBLEConstant(value.second, device->getDeviceContext(), commandQueue, i, kernel);
			break;
		case STRING:
			bufferToAdd = generateBufferForCHARConstant(value.second, device->getDeviceContext(), commandQueue, i, kernel);
			break;
		default:;
		}
//		std::cout << "Error Code for Setting Constant: " << ErrorCode<< std::endl;
		i++;
	}
}

cl::Buffer* Scheduler::generateBufferForUINT(std::vector<void*> data, cl::Context context, cl::CommandQueue queue, int count) {
	uint32_t* uintRamDataToAdd = new uint32_t[data.size()];

	for (long unsigned int i = 0; i < data.size(); i++)
	{
		uintRamDataToAdd[i] = *((uint32_t*)data.at(i));
	}

	cl::Buffer* buffer = new cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(uint32_t) * data.size());
	ErrorCode = queue.enqueueWriteBuffer(*buffer, CL_TRUE, count, sizeof(uint32_t) * data.size(), uintRamDataToAdd);
	return buffer;
}

cl::Buffer* Scheduler::generateBufferForINT(std::vector<void*> data, cl::Context context, cl::CommandQueue queue, int count) {
	int32_t* intRamDataToAdd = new int32_t[data.size()];

	for (long unsigned int i = 0; i < data.size(); i++)
	{
		intRamDataToAdd[i] = *((int32_t*)data.at(i));
	}

	cl::Buffer* buffer = new cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int32_t) * data.size());
	queue.enqueueWriteBuffer(*buffer, CL_TRUE, count, sizeof(int32_t) * data.size(), intRamDataToAdd);
	return buffer;
}

cl::Buffer* Scheduler::generateBufferForCHAR(std::vector<void*> data, cl::Context context, cl::CommandQueue queue, int count) {
	char* charRamDataToAdd = new char[data.size()];

	for (long unsigned int i = 0; i < data.size(); i++)
	{
		charRamDataToAdd[i] = *((char*)data.at(i));
	}

	cl::Buffer* buffer = new cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(char) * data.size());
	queue.enqueueWriteBuffer(*buffer, CL_TRUE, count, sizeof(char) * data.size(), charRamDataToAdd);
	return buffer;
}

cl::Buffer* Scheduler::generateBufferForDOUBLE(std::vector<void*> data, cl::Context context, cl::CommandQueue queue, int count) {
	double* doubleRamDataToAdd = new double[data.size()];

	for (long unsigned int i = 0; i < data.size(); i++)
	{
		doubleRamDataToAdd[i] = *((double*)data.at(i));
	}

	cl::Buffer* buffer = new cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * data.size());
	queue.enqueueWriteBuffer(*buffer, CL_TRUE, count, sizeof(double) * data.size(), doubleRamDataToAdd);
	return buffer;
}

cl::Buffer* Scheduler::generateBufferForFLOAT(std::vector<void*> data, cl::Context context, cl::CommandQueue queue, int count) {
	float* floatRamDataToAdd = new float[data.size()];

	for (long unsigned int i = 0; i < data.size(); i++)
	{
		floatRamDataToAdd[i] = *((float*)data.at(i));
	}

	cl::Buffer* buffer = new cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(float) * data.size());
	queue.enqueueWriteBuffer(*buffer, CL_TRUE, count, sizeof(float) * data.size(), floatRamDataToAdd);
	return buffer;
}

cl::Buffer* Scheduler::generateBufferForUINTConstant(void* data, cl::Context context, cl::CommandQueue queue, int count, cl::Kernel kernel)
{
	cl::Buffer* returnValue = new cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(unsigned int), &ErrorCode);
	//queue.enqueueWriteBuffer(*returnValue, CL_TRUE, count, sizeof(unsigned int), data);
	ErrorCode = kernel.setArg(count, *((unsigned int*)data));
	return returnValue;
}

cl::Buffer* Scheduler::generateBufferForINTConstant(void* data, cl::Context context, cl::CommandQueue queue, int count, cl::Kernel kernel)
{
	cl::Buffer* returnValue = new cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int), &ErrorCode);
	//queue.enqueueWriteBuffer(*returnValue, CL_TRUE, count, sizeof(int), data);
	ErrorCode = kernel.setArg(count, *((int*)data));
	return returnValue;
}

cl::Buffer* Scheduler::generateBufferForFLOATConstant(void* data, cl::Context context, cl::CommandQueue queue,
	int count, cl::Kernel kernel)
{
	int ErrorCode = 0;
	cl::Buffer* returnValue = new cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(float), &ErrorCode);
	//queue.enqueueWriteBuffer(*returnValue, CL_TRUE, count, sizeof(float), data);
	ErrorCode = kernel.setArg(count, *((float*)data));
	return returnValue;
}

cl::Buffer* Scheduler::generateBufferForDOUBLEConstant(void* data, cl::Context context, cl::CommandQueue queue,
	int count, cl::Kernel kernel)
{
	int ErrorCode = 0;
	cl::Buffer* returnValue = new cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(double), &ErrorCode);
	//queue.enqueueWriteBuffer(*returnValue, CL_TRUE, count, sizeof(double), data);
	ErrorCode = kernel.setArg(count, *((double*)data));
	return returnValue;
}

cl::Buffer* Scheduler::generateBufferForCHARConstant(void* data, cl::Context context, cl::CommandQueue queue, int count, cl::Kernel kernel)
{
	int ErrorCode = 0;
	cl::Buffer* returnValue = new cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(char), &ErrorCode);
	//queue.enqueueWriteBuffer(*returnValue, CL_TRUE, count, sizeof(char), data);
	ErrorCode = kernel.setArg(count, *((char*)data));
	return returnValue;
}

std::vector<void*> Scheduler::readDataFromBufferForUINT(Task* task, cl::CommandQueue queue, int count)
{
	uint32_t* data = new uint32_t[task->getLoad()];
	uint32_t* copiedData = new uint32_t[task->getLoad()];
	std::vector<void*> returnData;
	ErrorCode = queue.enqueueReadBuffer(*task->readBuffer(), CL_TRUE, count, sizeof(uint32_t) * task->getLoad(), data);
    ErrorCode = queue.finish();
	for (int i = 0; i < task->getLoad(); i++) {
		copiedData[i] = data[i];
		void* value = &copiedData[i];
		returnData.emplace_back(value);
	}
	return returnData;
}

std::vector<void*> Scheduler::readDataFromBufferForINT(Task* task, cl::CommandQueue queue, int count)
{
	int32_t* data = new int32_t[task->getLoad()];
	int32_t* copiedData = new int32_t[task->getLoad()];
	std::vector<void*> returnData;
    ErrorCode = queue.enqueueReadBuffer(*task->readBuffer(), CL_TRUE, count, sizeof(int32_t) * task->getLoad(), data);
    ErrorCode = queue.finish();
	for (int i = 0; i < task->getLoad(); i++) {
		copiedData[i] = data[i];
		void* value = &copiedData[i];
		returnData.emplace_back(value);
	}
	return returnData;
}

std::vector<void*> Scheduler::readDataFromBufferForCHAR(Task* task, cl::CommandQueue queue, int count)
{
	char* data = new char[task->getLoad()];
	char* copiedData = new char[task->getLoad()];
	std::vector<void*> returnData;
    ErrorCode =queue.enqueueReadBuffer(*task->readBuffer(), CL_TRUE, count, sizeof(char) * task->getLoad(), data);
    ErrorCode =queue.finish();
	for (int i = 0; i < task->getLoad(); i++) {
		copiedData[i] = data[i];
		void* value = &copiedData[i];
		returnData.emplace_back(value);
	}
	return returnData;
}

std::vector<void*> Scheduler::readDataFromBufferForDOUBLE(Task* task, cl::CommandQueue queue, int count)
{
	double* data = new double[task->getLoad()];
	double* copiedData = new double[task->getLoad()];
	std::vector<void*> returnData;
    ErrorCode =queue.enqueueReadBuffer(*task->readBuffer(), CL_TRUE, count, sizeof(double) * task->getLoad(), data);
    ErrorCode =queue.finish();
	for (int i = 0; i < task->getLoad(); i++) {
		copiedData[i] = data[i];
		void* value = &copiedData[i];
		returnData.emplace_back(value);
	}
	return returnData;
}

std::vector<void*> Scheduler::readDataFromBufferForFLOAT(Task* task, cl::CommandQueue queue, int count)
{
	float* data = new  float[task->getLoad()];
	float* copiedData = new float[task->getLoad()];
	std::vector<void*> returnData;
    ErrorCode =queue.enqueueReadBuffer(*task->readBuffer(), CL_TRUE, count, sizeof(float) * task->getLoad(), data);
    ErrorCode =queue.finish();

	for (int i = 0; i < task->getLoad(); i++) {
		copiedData[i] = data[i];
		void* value = &copiedData[i];
		returnData.emplace_back(value);
	}
	return returnData;
}

void Scheduler::setCoreCount(int cores) {
	CoreCount = cores;
}
