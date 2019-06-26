//
// Created by moritz on 17.06.19.
//

#include <stdint.h>


#include <iostream>
#include "Scheduler.h"

using namespace SCHEDULER;

Scheduler::Scheduler(std::vector<Task*> tasks, std::vector<Device> devices) {
    Tasks=tasks;
    Devices=devices;
}

void Scheduler::setRAMForCurrentTask(Task* task, Device device, cl::Kernel kernel, cl::CommandQueue queue) {
    int count = 0;
    for(std::pair<Type, std::vector<void*>> value : task->getAllData()){
        cl::Buffer *buffer;
        switch (value.first){
            case Type::UINT:
                buffer = generateBufferForUINT(value.second,device.getDeviceContext(),queue,0);
                break;
            case Type::INT:
                buffer = generateBufferForINT(value.second,device.getDeviceContext(),queue,0);
                break;
            case Type::CHAR:
                buffer = generateBufferForCHAR(value.second,device.getDeviceContext(),queue,0);
                break;
            case Type::DOUBLE:
                buffer = generateBufferForDOUBLE(value.second,device.getDeviceContext(),queue,0);
                break;
            case Type::FLOAT:
                buffer = generateBufferForFLOAT(value.second,device.getDeviceContext(),queue,0);
                break;
            case Type::STRING:
                buffer = generateBufferForCHAR(value.second,device.getDeviceContext(),queue,0);
                break;
            default:
				break;
        }
        int returnvlaue = kernel.setArg(count, *buffer);
        std::cout<<"Error Code: "<<returnvlaue;
		count++;
    }
}

void Scheduler::setRAMBufferForOutput(Task* task, Device device, cl::Kernel kernel)
{
	cl::Buffer *buffer;
	switch (task->getReturnDataType())
	{
	case Type::UINT:
		buffer = new cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(uint32_t) * task->getLoad());
		break;
	case Type::INT:
		buffer = new cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(int32_t) * task->getLoad());
		break;
	case Type::CHAR:
		buffer = new cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(char) * task->getLoad());
		break;
	case Type::DOUBLE:
		buffer = new cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(double) * task->getLoad());
		break;
	case Type::FLOAT:
		buffer = new cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(float) * task->getLoad());
		break;
	case Type::STRING:
		buffer = new cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(char) * task->getLoad());
		break;
	default:
		break;
	}
	task->setReadBuffer(buffer);
	kernel.setArg(task->getAllData().size(), *buffer);
}

void Scheduler::setKernelLoad(Task* task, Device device, cl::Kernel kernel)
{
	int ErrorCode=0;
	cl::Buffer* buffer_WORKLOAD = new cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(int), &ErrorCode);
	kernel.setArg(task->getAllData().size()+1,task->getLoad());
	std::cout<<"ErrorCode: "<<ErrorCode;
}

void Scheduler::enqueueTak(Task* task, Device device, cl::CommandQueue commandQueue, cl::Kernel kernel)
{
	commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(1),cl::NDRange(1));
	commandQueue.finish();
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
	task->setReturnData(data);
}

cl::Buffer* Scheduler::generateBufferForUINT(std::vector<void*> data, cl::Context context, cl::CommandQueue queue, int count) {
	uint32_t* uintRamDataToAdd = new uint32_t[data.size()];

	for (long unsigned int i = 0; i < data.size(); i++)
	{
	    std::cout<<"Value: "<<*((uint32_t*)data.at(i))<<std::endl;
		uintRamDataToAdd[i] = *((uint32_t*)data.at(i));
	}

	std::cout<<"RamSize"<<sizeof(uint32_t)*data.size();

    cl::Buffer *buffer=new cl::Buffer(context,CL_MEM_READ_WRITE,sizeof(uint32_t)*data.size());
    int errorCode = queue.enqueueWriteBuffer(*buffer, CL_TRUE, count, sizeof(uint32_t) * data.size(), uintRamDataToAdd);
    std::cout <<"Error code: "<<errorCode;
    return buffer;
}

cl::Buffer* Scheduler::generateBufferForINT(std::vector<void*> data, cl::Context context, cl::CommandQueue queue, int count) {
    int32_t* intRamDataToAdd = new int32_t[data.size()];

	for (long unsigned int i = 0; i < data.size(); i++)
	{
		intRamDataToAdd[i] = *((int32_t*)data.at(i));
	}

    cl::Buffer* buffer=new cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int32_t) * data.size());
    queue.enqueueWriteBuffer(*buffer,CL_TRUE,count, sizeof(int32_t) * data.size(),intRamDataToAdd);
    return buffer;
}

cl::Buffer* Scheduler::generateBufferForCHAR(std::vector<void*> data, cl::Context context, cl::CommandQueue queue, int count) {
    char* charRamDataToAdd = new char[data.size()];

	for (long unsigned int i = 0; i < data.size(); i++)
	{
		charRamDataToAdd[i] = *((char*)data.at(i));
	}

    cl::Buffer *buffer = new cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(char) * data.size());
    queue.enqueueWriteBuffer(*buffer,CL_TRUE,count, sizeof(char) * data.size(),charRamDataToAdd);
    return buffer;
}

cl::Buffer* Scheduler::generateBufferForDOUBLE(std::vector<void*> data, cl::Context context, cl::CommandQueue queue, int count) {
    double* doubleRamDataToAdd = new double[data.size()];

	for (long unsigned int i = 0; i < data.size(); i++)
	{
		doubleRamDataToAdd[i] = *((double*)data.at(i));
	}

    cl::Buffer *buffer = new cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(double) * data.size());
    queue.enqueueWriteBuffer(*buffer,CL_TRUE,count, sizeof(double) * data.size(),doubleRamDataToAdd);
    return buffer;
}

cl::Buffer* Scheduler::generateBufferForFLOAT(std::vector<void*> data, cl::Context context, cl::CommandQueue queue, int count) {
    float* floatRamDataToAdd = new float[data.size()];

	for (long unsigned int i = 0; i < data.size(); i++)
	{
		floatRamDataToAdd[i] = *((float*)data.at(i));
	}

    cl::Buffer *buffer = new cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(float) * data.size());
    queue.enqueueWriteBuffer(*buffer,CL_TRUE,count, sizeof(float) * data.size(), floatRamDataToAdd);
    return buffer;
}

std::vector<void*> Scheduler::readDataFromBufferForUINT(Task* task, cl::CommandQueue queue, int count)
{
	uint32_t* data = new uint32_t[task->getLoad()];
	uint32_t* copiedData = new uint32_t[task->getLoad()];
	std::vector<void*> returnData;
	queue.enqueueReadBuffer(*task->readBuffer(), CL_TRUE,count, sizeof(uint32_t) * task->getLoad(),data);
	queue.finish();
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
	queue.enqueueReadBuffer(*task->readBuffer(), CL_TRUE, count, sizeof(int32_t) * task->getLoad(), data);
	queue.finish();
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
	queue.enqueueReadBuffer(*task->readBuffer(), CL_TRUE, count, sizeof(char) * task->getLoad(), data);
	queue.finish();
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
	queue.enqueueReadBuffer(*task->readBuffer(), CL_TRUE, count, sizeof(double) * task->getLoad(), data);
	queue.finish();
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
	queue.enqueueReadBuffer(*task->readBuffer(), CL_TRUE, count, sizeof(float) * task->getLoad(), data);
	queue.finish();

	for (int i = 0; i < task->getLoad(); i++) {
		copiedData[i] = data[i];
		void* value = &copiedData[i];
		returnData.emplace_back(value);
	}
	return returnData;
}
