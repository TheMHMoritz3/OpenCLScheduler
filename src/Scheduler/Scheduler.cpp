//
// Created by moritz on 17.06.19.
//

#ifdef _WINDOWS
#include <stdint.h>
#endif // _WINDOWS


#include "Scheduler.h"

using namespace SCHEDULER;

Scheduler::Scheduler(std::vector<Task*> tasks, std::vector<Device> devices) {
    Tasks=tasks;
    Devices=devices;
}

void Scheduler::setRAMForCurrentTask(Task* task, Device device, cl::Kernel kernel, cl::CommandQueue queue) {
    int i = 0;
    for(std::pair<Type,void*> value : task->getAllData()){
        cl::Buffer buffer;
        switch (value.first){
            case Type::UINT:
                buffer = generateBufferForUINT(value.second,device.getDeviceContext(),queue,i);
                break;
            case Type::INT:
                buffer = generateBufferForINT(value.second,device.getDeviceContext(),queue,i);
                break;
            case Type::CHAR:
                buffer = generateBufferForCHAR(value.second,device.getDeviceContext(),queue,i);
                break;
            case Type::DOUBLE:
                buffer = generateBufferForDOUBLE(value.second,device.getDeviceContext(),queue,i);
                break;
            case Type::FLOAT:
                buffer=generateBufferForFLOAT(value.second,device.getDeviceContext(),queue,i);
                break;
            case Type::STRING:
                buffer = generateBufferForCHAR(value.second,device.getDeviceContext(),queue,i);
                break;
            default:
                break;
        }
        kernel.setArg(i, buffer);
        i++;
    }
}

void Scheduler::setRAMBufferForOutput(Task* task, Device device, cl::Kernel kernel)
{
	cl::Buffer buffer;
	switch (task->getReturnDataType())
	{
	case Type::UINT:
#ifdef _WINDOWS
		buffer = cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(uint32_t) * task->getLoad());
#endif // _WINDOWS
#ifndef _WINDOWS
		buffer = cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(u_int32_t) * task->getLoad());
#endif // !_WINDOWS
		break;
	case Type::INT:
		buffer = cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(int32_t) * task->getLoad());
		break;
	case Type::CHAR:
		buffer = cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(char) * task->getLoad());
		break;
	case Type::DOUBLE:
		buffer = cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(double) * task->getLoad());
		break;
	case Type::FLOAT:
		buffer = cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(float) * task->getLoad());
		break;
	case Type::STRING:
		buffer = cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(char) * task->getLoad());
		break;
	default:
		break;
	}
	task->setReadBuffer(buffer);
	kernel.setArg(task->getAllData().size(), buffer);
}

void Scheduler::setKernelLoad(Task* task, Device device, cl::Kernel kernel)
{
	cl::Buffer buffer_WORKLOAD(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(int));
	kernel.setArg(task->getAllData().size(), task->getLoad());
}

void Scheduler::enqueueTak(Task* task, Device device, cl::CommandQueue commandQueue, cl::Kernel kernel)
{
	commandQueue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(task->getAllData().size()),cl::NDRange(device.getMaxComputeUnits()));
	commandQueue.finish();
}

void Scheduler::readDataFromTask(Task* task, cl::CommandQueue commandQueue)
{
	void* data;
	switch (task->getReturnDataType())
	{
	case Type::UINT:
		data = readDataFromBufferForUINT(task, commandQueue, task->getAllData().size());
		break;
	case Type::INT:
		data = readDataFromBufferForINT(task, commandQueue, task->getAllData().size());
		break;
	case Type::CHAR:
		data = readDataFromBufferForCHAR(task, commandQueue, task->getAllData().size());
		break;
	case Type::DOUBLE:
		data = readDataFromBufferForDOUBLE(task, commandQueue, task->getAllData().size());
		break;
	case Type::FLOAT:
		data = readDataFromBufferForFLOAT(task, commandQueue, task->getAllData().size());
		break;
	case Type::STRING:
		data = readDataFromBufferForCHAR(task, commandQueue, task->getAllData().size());
		break;
	default:
		break;
	}
	task->setReturnData(data);
}

cl::Buffer Scheduler::generateBufferForUINT(void *data, cl::Context context, cl::CommandQueue queue, int count) {
#ifdef _WINDOWS
	uint32_t* uintRamDataToAdd = (uint32_t*)data;
#endif // _WINDOWS
#ifndef _WINDOWS
	u_int32_t* uintRamDataToAdd = (u_int32_t*)data;
#endif // !_WINDOWS
    cl::Buffer buffer(context,CL_MEM_READ_WRITE,sizeof(*uintRamDataToAdd));
    queue.enqueueWriteBuffer(buffer, CL_TRUE, count, sizeof(*uintRamDataToAdd), uintRamDataToAdd);
    return buffer;
}

cl::Buffer Scheduler::generateBufferForINT(void *data, cl::Context context, cl::CommandQueue queue, int count) {
    int32_t* intRamDataToAdd = (int32_t *)data;
    cl::Buffer buffer(context, CL_MEM_READ_WRITE, sizeof(*intRamDataToAdd));
    queue.enqueueWriteBuffer(buffer,CL_TRUE,count, sizeof(*intRamDataToAdd),intRamDataToAdd);
    return buffer;
}

cl::Buffer Scheduler::generateBufferForCHAR(void *data, cl::Context context, cl::CommandQueue queue, int count) {
    char* charRamDataToAdd = (char *)data;
    cl::Buffer buffer(context, CL_MEM_READ_WRITE, sizeof(*charRamDataToAdd));
    queue.enqueueWriteBuffer(buffer,CL_TRUE,count, sizeof(*charRamDataToAdd),charRamDataToAdd);
    return buffer;
}

cl::Buffer Scheduler::generateBufferForDOUBLE(void *data, cl::Context context, cl::CommandQueue queue, int count) {
    double* doubleRamDataToAdd = (double *)data;
    cl::Buffer buffer(context, CL_MEM_READ_WRITE, sizeof(*doubleRamDataToAdd));
    queue.enqueueWriteBuffer(buffer,CL_TRUE,count, sizeof(*doubleRamDataToAdd),doubleRamDataToAdd);
    return buffer;
}

cl::Buffer Scheduler::generateBufferForFLOAT(void *data, cl::Context context, cl::CommandQueue queue, int count) {
    float* doubleRamDataToAdd = (float *)data;
    cl::Buffer buffer(context, CL_MEM_READ_WRITE, sizeof(*doubleRamDataToAdd));
    queue.enqueueWriteBuffer(buffer,CL_TRUE,count, sizeof(*doubleRamDataToAdd),doubleRamDataToAdd);
    return buffer;
}

void* Scheduler::readDataFromBufferForUINT(Task* task, cl::CommandQueue queue, int count)
{
#ifdef _WINDOWS
	uint32_t* data = new uint32_t[task->getLoad()];
	uint32_t* copiedData = new uint32_t[task->getLoad()];
	queue.enqueueReadBuffer(task->readBuffer(), CL_TRUE,count, sizeof(uint32_t) * task->getLoad(),data);
#endif // _WINDOWS
#ifndef _WINDOWS
	u_int32_t* data = new u_int32_t[task->getLoad()];
	u_int32_t* copiedData = new u_int32_t[task->getLoad()];
	queue.enqueueReadBuffer(task->readBuffer(), CL_TRUE, count, sizeof(u_int32_t) * task->getLoad(), data);
#endif // !_WINDOWS
	queue.finish();
	for (int i = 0; i < task->getLoad(); i++) {
		copiedData[i] = data[i];
	}
	void* voidData = copiedData;
	return voidData;
}

void* Scheduler::readDataFromBufferForINT(Task* task, cl::CommandQueue queue, int count)
{
	int32_t* data = new int32_t[task->getLoad()];
	int32_t* copiedData = new int32_t[task->getLoad()];
	queue.enqueueReadBuffer(task->readBuffer(), CL_TRUE, count, sizeof(int32_t) * task->getLoad(), data);
	queue.finish();
	for (int i = 0; i < task->getLoad(); i++) {
		copiedData[i] = data[i];
	}
	void* voidData = copiedData;
	return voidData;
}

void* Scheduler::readDataFromBufferForCHAR(Task* task, cl::CommandQueue queue, int count)
{
	char* data = new char[task->getLoad()];
	char* copiedData = new char[task->getLoad()];
	queue.enqueueReadBuffer(task->readBuffer(), CL_TRUE, count, sizeof(char) * task->getLoad(), data);
	queue.finish();
	for (int i = 0; i < task->getLoad(); i++) {
		copiedData[i] = data[i];
	}
	void* voidData = copiedData;
	return voidData;
}

void* Scheduler::readDataFromBufferForDOUBLE(Task* task, cl::CommandQueue queue, int count)
{
	double* data = new double[task->getLoad()];
	double* copiedData = new double[task->getLoad()];
	queue.enqueueReadBuffer(task->readBuffer(), CL_TRUE, count, sizeof(double) * task->getLoad(), data);
	queue.finish();
	for (int i = 0; i < task->getLoad(); i++) {
		copiedData[i] = data[i];
	}
	void* voidData = copiedData;
	return voidData;
}

void* Scheduler::readDataFromBufferForFLOAT(Task* task, cl::CommandQueue queue, int count)
{
	float* data = new  float[task->getLoad()];
	float* copiedData = new float[task->getLoad()];
	queue.enqueueReadBuffer(task->readBuffer(), CL_TRUE, count, sizeof(float) * task->getLoad(), data);
	queue.finish();
	for (int i = 0; i < task->getLoad(); i++) {
		copiedData[i] = data[i];
	}
	void* voidData = copiedData;
	return voidData;
}
