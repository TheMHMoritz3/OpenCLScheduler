//
// Created by moritz on 17.06.19.
//

#include "StaticScheduler.h"
#include <iostream>
#include <string>
#include <cstring>
#include <CL/cl.hpp>
#include <fstream>

using namespace SCHEDULER;
using namespace std;

StaticScheduler::StaticScheduler(std::vector<Task*> tasks, std::vector<Device> device) : Scheduler(tasks, device) {
	ErrorCode = 1;
}


void StaticScheduler::schedule() {
	for (Device device : Devices) {
		cout << "Device Name: " << device.getName();
		cl::CommandQueue commandQueue(device.getDeviceContext(), device.getOclDevice(), ErrorCode);
		CommandQueues.push_back(commandQueue);
		for (Task* task : Tasks) {
			device.generateProgramm(task);


//            cl_int ret = 1;
//            float B[task->getLoad()] = {};
//
//            cl::CommandQueue queue(device.getDeviceContext(),device.getOclDevice(), ret);
//            cl::Kernel kernel=cl::Kernel(task->getProgramm(),"xAxis");
//
//            int appendedBuffers = 0;
//
//            uint32_t *A;
//
//            for(unsigned long i =0 ; i<task->getUintValues()->size(); i++) {
//                A = task->getUintValues()->at(i);
//
//                cl::Buffer *buffer_A = new cl::Buffer(device.getDeviceContext(), CL_MEM_READ_WRITE, sizeof(uint32_t) * task->getLoad());
//                ret = queue.enqueueWriteBuffer(*buffer_A, CL_TRUE, 0, sizeof(uint32_t) * task->getLoad(), A);
//                kernel.setArg(appendedBuffers, *buffer_A);
//                appendedBuffers++;
//
//                //---Debug---
//                ret = queue.finish();
//                printf("Kernel Success ArgSet %d\n", ret);
//            }
//
//
////            cl::Buffer buffer_A(device.getDeviceContext(),CL_MEM_READ_WRITE,sizeof(uint32_t)*task->getLoad());
//            cl::Buffer buffer_B(device.getDeviceContext(),CL_MEM_READ_WRITE,sizeof(float)*task->getLoad());
//            cl::Buffer buffer_WORKLOAD(device.getDeviceContext(),CL_MEM_READ_WRITE,sizeof(int));
//
//
//            //---Debug---
//            ret = queue.finish();
//            printf("Kernel Success CmdQ %d\n", ret);
//
//
//            //---Debug---
//            //ret = queue.finish();
//            //printf("Kernel Success WriteBuffer %d\n", ret);
//
//
//
//            kernel.setArg(appendedBuffers,buffer_B);
//            appendedBuffers++;
//            kernel.setArg(appendedBuffers,task->getLoad());
//
//            //---Debug---
//            ret = queue.finish();
//            printf("Kernel Success ArgSet %d\n", ret);
//
//            ret = queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(1),cl::NDRange(1));
//            queue.finish();
//            //---Debug---
//            printf("Kernel Success NDRange %d\n", ret);
//
//            queue.enqueueReadBuffer(buffer_B,CL_TRUE,0,sizeof(float)*task->getLoad(),B);
//            queue.finish();
//
//
//            for(int i = 0; i < task->getLoad(); i++)
//            {
//                cout << "returnValue: " << B[i] << "\n";
//            }


			cl::Kernel kernel = cl::Kernel(task->getProgramm(), task->getKernelName().c_str(),  &ErrorCode);
			if (ErrorCode == CL_SUCCESS) {
				setRAMForCurrentTask(task, device, kernel, commandQueue);
				setRAMBufferForOutput(task, device, kernel);
				setKernelLoad(task, device, kernel);
				enqueueTak(task, device, commandQueue, kernel);
				readDataFromTask(task, commandQueue);
			}
			else
				cout << "Kernel Creation Resolved Error: " << ErrorCode;
		}
	}
}

