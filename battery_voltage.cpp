#include <iostream>
#include <fstream>
#include <CL/cl.hpp>
#include <chrono>  // for high_resolution_clock
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <c3can/error/extended.h>
#include <c3can/core.h>

#define U32_DATA(p) (*(uint32_t*)(p))

cl::Device settingUpDevice(int platform){
    //get all platforms (drivers)
    std::vector<cl::Platform> all_platforms;
    char platformName[64];
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Platform default_platform=all_platforms[platform];    
    std::cout << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";
    
    
 
    //get default device of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Device default_device=all_devices[0];
    
    return default_device;
 
}

cl::Program settingUpProgram(cl::Device default_device, cl::Context context){
 
    cl::Program::Sources sources;
 
    std::ifstream sourceFile("kernel.cl");
    std::string kernel_code(std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));
    sources.push_back({kernel_code.c_str(),kernel_code.length()});
 
    cl::Program program(context,sources);
    if(program.build({default_device})!=CL_SUCCESS){
        std::cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)<<"\n";
        exit(1);
    }
    
    return program;
}

double run(int load, int count, uint32_t* items, int cores,cl::Device default_device, cl::Context context, cl::Program program){
    
    uint32_t* A = items;
    cl_int ret = 1;
    float B[count] = {};
     
    auto start = std::chrono::high_resolution_clock::now();
    
    cl::Buffer buffer_A(context,CL_MEM_READ_WRITE,sizeof(uint32_t)*count);
    cl::Buffer buffer_B(context,CL_MEM_READ_WRITE,sizeof(float)*count);
    cl::Buffer buffer_WORKLOAD(context,CL_MEM_READ_WRITE,sizeof(int));
        
    cl::CommandQueue queue(context,default_device, ret);    
    //---Debug---
    //ret = queue.finish();    
    //printf("Kernel Success CmdQ %d\n", ret);
 
    ret = queue.enqueueWriteBuffer(buffer_A,CL_TRUE,0,sizeof(uint32_t)*count,items);    
    //---Debug---
    //ret = queue.finish();
    //printf("Kernel Success WriteBuffer %d\n", ret);
 
    cl::Kernel battery=cl::Kernel(program,"batteryCalc");
    battery.setArg(0,buffer_A);
    battery.setArg(1,buffer_B);
    battery.setArg(2,load);
        
    //---Debug---
    //ret = queue.finish();    
    //printf("Kernel Success ArgSet %d\n", ret);   
    
    ret = queue.enqueueNDRangeKernel(battery,cl::NullRange,cl::NDRange(count/load),cl::NDRange(cores));    
    queue.finish();
    //---Debug---
    //printf("Kernel Success NDRange %d\n", ret);    

    queue.enqueueReadBuffer(buffer_B,CL_TRUE,0,sizeof(float)*count,B);
    queue.finish();
    
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    
    for(int i = 0; i < count; i++)
        {
            std::cout << "Voltage: " << B[i]<<  "V" << "\n";            
		}

    return elapsed.count();
}

double run_no_lib(int count, float* items){
    float B[count] = {};
    float C[count] = {};
    float dummy;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for(int i = 0; i< count; i++){
        for (int j = 0; j < 100; ++j){
			B[i] = items[i] * (1024/ (4.9f * 5));
			dummy = (100000.412313f/3333.85764f) * (1000.6123f/123123.9874f) * 6729.4297f;
			C[(j + i + 1) % count] = dummy / (j * 123.1231231f) ;
			C[(j + i + 1) % count]= dummy * (items[(j + i) % count]) * (1024.0f / (4.9f * 5.0f));
			C[(j + i) % count]= (items[(j + i) % count]) * (1024.0f / (4.9f * 5.0f));
		}
    }
    
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;

    return elapsed.count();
}

uint32_t* create_data(int size) // size: Datapoints collected from canBus //stretch: Datapoints stretched to this size
{
    uint32_t* col_res = new uint32_t[size];   
    
    std::cout << "Setting up C3CAN\n";
    c3can_single *single = c3can_single_init("can0");
    C3CAN_CHECK_ERR(single, exit, -1);    
    c3can_single_filter_add(single, 0x0d, (C3CAN_SINGLE_FILTER_OPTS) 0);
    /* We're receiving blocking */
    c3can_message msg;
    /* we're requesting the hardware timestamp for better documentation */
    struct timeval timestamp;    
    
    std::cout << "Collecting Data...\n";    
	for(int i = 0; i < size; i++) 
	{
		c3can_single_recv(single, &msg, &timestamp);
        C3CAN_CHECK_ERR(single, exit, -1);
		col_res[i] = U32_DATA(c3can_message_get_payload(&msg));		
	}
	
    std::cout << "Finished Data!\n";
    return col_res;
}

int main(){
    
    const int DEFAULT_SIZE = 10;   
    double execTimeVCL, execTimePOCL;     
    
    cl::Device default_device = settingUpDevice(0); // 0 = VideoCore IV ; 1 = POCL on CPU
    cl::Context context({default_device});
    cl::Device default_device2 = settingUpDevice(1); // 0 = VideoCore IV ; 1 = POCL on CPU
    cl::Context context2({default_device2});
    std::cout << "Setting up VC4CL OpenCl Programs\n";
    cl::Program program = settingUpProgram(default_device, context);
        
    std::cout << "Setting up POCL OpenCl Programs\n";
    cl::Program program2 = settingUpProgram(default_device2, context2);

	uint32_t* data = create_data(DEFAULT_SIZE);
	for(int i = 0; i < DEFAULT_SIZE; i++) 
	{
		std::cout << std::hex << "Data Point: " << i << ": " << (uint32_t)data[i]<< "\n";
	}
	
	std::cout << "Computing on GPU - VC4CL" << std::endl;
	execTimeVCL = run(1, DEFAULT_SIZE, data, DEFAULT_SIZE, default_device, context, program);
	std::cout << "Computing on CPU - POCL" << std::endl;
	execTimePOCL = run(1, DEFAULT_SIZE, data, DEFAULT_SIZE, default_device2, context2, program2);    
    
    return 0;
}
