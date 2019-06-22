//
// Created by moritz on 17.06.19.
//

#include "Scheduler.h"

using namespace SCHEDULER;

Scheduler::Scheduler(std::vector<Task> tasks, std::vector<Device> devices) {
    Tasks=tasks;
    Devices=devices;
}

void Scheduler::setRAMForCurrentTask(Task task, Device device, cl::Kernel kernel, cl::CommandQueue queue) {
    int i = 0;
    for(std::pair<Type,void*> value : task.getAllData()){
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

cl::Buffer Scheduler::generateBufferForUINT(void *data, cl::Context context, cl::CommandQueue queue, int count) {
    u_int32_t *uintRamDataToAdd = (u_int32_t *) data;
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
