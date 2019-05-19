#include <iostream>
#include <fstream>
#include <CL/cl.hpp>
#include <chrono>  // for high_resolution_clock
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <utility>
#include <exception>

#include <c3can/error/extended.h>
#include <c3can/core.h>

#define U32_DATA(p) (*(uint32_t*)(p))

using namespace cl;
using namespace std;

/**
 * Sets up the Device for the Program with a specific ID
 * @param platform
 * @return The Device specified on the HardwareID
 */
Device settingUpDevice(int platform){
    //get all platforms (drivers)
    vector<Platform> all_platforms;
    char platformName[64];
    Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }
    Platform default_platform=all_platforms[platform];
    cout << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";



    //get default device of the default platform
    vector<Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    Device default_device=all_devices[0];

    return default_device;

}

/**
 * Generates the Program from the Kernelfile and the device propeties
 * @param default_device
 * @param context
 * @return
 */
Program settingUpProgram(Device default_device, Context context){

    Program::Sources sources;

    ifstream sourceFile("wheel_spin_kernel.cl");
    string kernel_code(istreambuf_iterator<char>(sourceFile), (istreambuf_iterator<char>()));
    sources.push_back({kernel_code.c_str(),kernel_code.length()});

    Program program(context,sources);
    if(program.build({default_device})!=CL_SUCCESS){
        cout<<" Error building: "<<program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)<<"\n";
        exit(1);
    }

    return program;
}

/**
 * Executes the content of the kernel file.
 * @param load
 * @param count
 * @param items
 * @param cores
 * @param default_device
 * @param context
 * @param program
 * @return
 */
pair<double,float*> runSpeedCalculation(int load, int count, uint32_t* items, int cores, Device default_device, Context context, Program program){

    uint32_t* A = items;
    cl_int ret = 1;
    float B[count] = {};

    auto start = chrono::high_resolution_clock::now();
    cout << "creating buffers" << endl;

    Buffer buffer_A(context,CL_MEM_READ_WRITE,sizeof(uint32_t)*count);
    Buffer buffer_B(context,CL_MEM_READ_WRITE,sizeof(float)*count);
    Buffer buffer_WORKLOAD(context,CL_MEM_READ_WRITE,sizeof(int));
    cout << "creating buffers finished staring queue"<<endl;

    CommandQueue queue(context,default_device, ret);
    //---Debug---
    //ret = queue.finish();
    //printf("Kernel Success CmdQ %d\n", ret);

    ret = queue.enqueueWriteBuffer(buffer_A,CL_TRUE,0,sizeof(uint32_t)*count,items);
    //---Debug---
//    ret = queue.finish();
//    printf("Kernel Success WriteBuffer %d\n", ret);

    Kernel speedCalculationWheel=Kernel(program,"speedCalcul");
    speedCalculationWheel.setArg(0,buffer_A);
    speedCalculationWheel.setArg(1,buffer_B);
    speedCalculationWheel.setArg(2,load);

    //---Debug---
//    ret = queue.finish();
//    printf("Kernel Success ArgSet %d\n", ret);

    try{
    ret = queue.enqueueNDRangeKernel(speedCalculationWheel,NullRange,NDRange(count/load),NDRange(cores));
    }catch(exception ex){
      cout<<ex.what();
    }

    queue.finish();
    //---Debug---
//    printf("Kernel Success NDRange %d\n", ret);

    queue.enqueueReadBuffer(buffer_B,CL_TRUE,0,sizeof(float)*count,B);
    queue.finish();

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
    
    float* speed = new float[count];
    for(int i = 0; i < count; i++)
    {
        speed[i] = B[i]; 
        cout << "Speed: " << speed[i] <<  "m/s" << "\n";
    }
    double elapsedTime = elapsed.count();
    pair<double,float*> returnvalue(elapsedTime,speed);
    return returnvalue;
}

/**
 * Executes the content of the kernel file.
 * @param load
 * @param count
 * @param items
 * @param cores
 * @param default_device
 * @param context
 * @param program
 * @return
 */
double runMedian(int load, int count, float* speed1, float* speed2, int cores, Device default_device, Context context, Program program){

    float *speedfront1 = new float[count];
    float *speedfront2 = new float[count];

    for(int i = 0; i<count; i++){
        speedfront1[i]=speed1[i];
        speedfront2[i]=speed2[i];
    }

    cl_int ret = 1;
    float B[count] = {};

    auto start = chrono::high_resolution_clock::now();

    Buffer buffer_speed1(context,CL_MEM_READ_WRITE,sizeof(float)*count);
    Buffer buffer_speed2(context,CL_MEM_READ_WRITE,sizeof(float)*count);
    Buffer buffer_B(context,CL_MEM_READ_WRITE,sizeof(float)*count);
    Buffer buffer_WORKLOAD(context,CL_MEM_READ_WRITE,sizeof(int));

    CommandQueue queue(context,default_device, ret);
    //---Debug---
    //ret = queue.finish();
    //printf("Kernel Success CmdQ %d\n", ret);

    ret = queue.enqueueWriteBuffer(buffer_speed1,CL_TRUE,0,sizeof(float)*count,speedfront1);
    ret = queue.enqueueWriteBuffer(buffer_speed2,CL_TRUE,0,sizeof(float)*count,speedfront2);
    //---Debug---
    //ret = queue.finish();
    //printf("Kernel Success WriteBuffer %d\n", ret);

    Kernel speedCalculationWheel=Kernel(program,"median");
    speedCalculationWheel.setArg(0,buffer_speed1);
    speedCalculationWheel.setArg(1,buffer_speed2);
    speedCalculationWheel.setArg(2,buffer_B);
    speedCalculationWheel.setArg(3,load);

    //---Debug---
//    ret = queue.finish();
//    printf("Kernel Success ArgSet %d\n", ret);

    ret = queue.enqueueNDRangeKernel(speedCalculationWheel,NullRange,NDRange(count/load),NDRange(cores));
    queue.finish();
    //---Debug---
//    printf("Kernel Success NDRange %d\n", ret);

    queue.enqueueReadBuffer(buffer_B,CL_TRUE,0,sizeof(float)*count,B);
    queue.finish();

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;

    for(int i = 0; i < count; i++)
    {
        cout << "Median: " << B[i]<<  "m/s" << "\n";
    }

    return elapsed.count();
}

double run_no_lib(int count, float* items){
    float B[count] = {};
    float C[count] = {};
    float dummy;

    auto start = chrono::high_resolution_clock::now();

    for(int i = 0; i< count; i++){
        for (int j = 0; j < 100; ++j){
            B[i] = items[i] * (1024/ (4.9f * 5));
            dummy = (100000.412313f/3333.85764f) * (1000.6123f/123123.9874f) * 6729.4297f;
            C[(j + i + 1) % count] = dummy / (j * 123.1231231f) ;
            C[(j + i + 1) % count]= dummy * (items[(j + i) % count]) * (1024.0f / (4.9f * 5.0f));
            C[(j + i) % count]= (items[(j + i) % count]) * (1024.0f / (4.9f * 5.0f));
        }
    }

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;

    return elapsed.count();
}

/**
 * Reads the data from the Bus
 * @param size
 * @return
 */
uint32_t* gatherDataFrontLeft(int size) // size: Datapoints collected from canBus //stretch: Datapoints stretched to this size
{
    uint32_t* col_res = new uint32_t[size];

    cout << "Setting up C3CAN\n";
    c3can_single *single = c3can_single_init("can0");
    C3CAN_CHECK_ERR(single, exit, -1);
    c3can_single_filter_add(single, 0x0d, (C3CAN_SINGLE_FILTER_OPTS) 0);
    /* We're receiving blocking */
    c3can_message msg;
    /* we're requesting the hardware timestamp for better documentation */
    struct timeval timestamp;

    cout << "Collecting Data - Front Left\n";
    for(int i = 0; i < size; i++)
    {
        c3can_single_recv(single, &msg, &timestamp);
        C3CAN_CHECK_ERR(single, exit, -1);
        col_res[i] = U32_DATA(c3can_message_get_payload(&msg));
    }

    cout << "Finished Data!\n";

    return col_res;
}

uint32_t* gatherDataFrontRight(int size) // size: Datapoints collected from canBus //stretch: Datapoints stretched to this size
{
    uint32_t* col_res = new uint32_t[size];

    cout << "Setting up C3CAN\n";
    c3can_single *single = c3can_single_init("can0");
    C3CAN_CHECK_ERR(single, exit, -1);
    c3can_single_filter_add(single, 0x0d, (C3CAN_SINGLE_FILTER_OPTS) 0);
    /* We're receiving blocking */
    c3can_message msg;
    /* we're requesting the hardware timestamp for better documentation */
    struct timeval timestamp;

    cout << "Collecting Data - Front Right\n";
    for(int i = 0; i < size; i++)
    {
        c3can_single_recv(single, &msg, &timestamp);
        C3CAN_CHECK_ERR(single, exit, -1);
        col_res[i] = U32_DATA(c3can_message_get_payload(&msg));
    }

    cout << "Finished Data!\n";

    return col_res;
}

int main(){

    const int DEFAULT_SIZE = 10;
    double execTimeVCL, execTimePOCL;

    Device default_device = settingUpDevice(0); // 0 = VideoCore IV ; 1 = POCL on CPU
    Context context({default_device});
    Device default_device2 = settingUpDevice(1); // 0 = VideoCore IV ; 1 = POCL on CPU
    Context context2({default_device2});
    cout << "Setting up VC4CL OpenCl Programs\n";
    Program program = settingUpProgram(default_device, context);

    cout << "Setting up POCL OpenCl Programs\n";
    Program program2 = settingUpProgram(default_device2, context2);

    uint32_t* data = gatherDataFrontLeft(DEFAULT_SIZE);
    for(int i = 0; i < DEFAULT_SIZE; i++)
    {
        cout << hex << "Data Point: " << i << ": " << (uint32_t)data[i]<< "\n";
    }

//    pair<double,float*> calculationValue;
    cout << "Computing Front Left on GPU - VC4CL" << endl;
    pair<double,float*> calculationValue = runSpeedCalculation(1, DEFAULT_SIZE, data, DEFAULT_SIZE, default_device, context, program);
    execTimeVCL = calculationValue.first;
    float* frontLeftValues=calculationValue.second;
    cout << "execution time: "<<execTimeVCL << "s" << endl;

    cout << "Computing Front Left on CPU - POCL" << endl;
    calculationValue = runSpeedCalculation(1, DEFAULT_SIZE, data, DEFAULT_SIZE, default_device2, context2, program2);
    execTimePOCL = calculationValue.first;
//    float* frontLeftValues=calculationValue.second;
    cout << "execution time: "<<execTimePOCL<<"s"<<endl;

    data = gatherDataFrontRight(DEFAULT_SIZE);
    for(int i = 0; i < DEFAULT_SIZE; i++)
    {
        cout << hex << "Data Point: " << i << ": " << (uint32_t)data[i]<< "\n";
    }

    cout << "Computing Front Right on GPU - VC4CL" << endl;
    calculationValue = runSpeedCalculation(1, DEFAULT_SIZE, data, DEFAULT_SIZE, default_device, context, program);
    execTimeVCL = calculationValue.first;
    float* frontRightValues=calculationValue.second;
    cout << "execution time: "<<execTimeVCL << "s" << endl;

    cout << "Computing Front Right on CPU - POCL" << endl;
    calculationValue = runSpeedCalculation(1, DEFAULT_SIZE, data, DEFAULT_SIZE, default_device2, context2, program2);
    execTimePOCL = calculationValue.first;
//    float* frontRightValues=calculationValue.second;
    cout << "execution time: "<<execTimePOCL<<"s"<<endl;

    cout << "Computing Median on GPU - VC4CL" << endl;
    execTimeVCL = runMedian(1, DEFAULT_SIZE, frontLeftValues,frontRightValues, DEFAULT_SIZE, default_device, context, program);
    cout << "execution time: "<<execTimeVCL << "s" << endl;
    
    for(int i = 0; i<DEFAULT_SIZE; i++){
        cout<<frontLeftValues[i]<<" "<<frontRightValues[i]<<endl;
    }
    cout << "Computing Median on CPU - POCL" << endl;
    execTimePOCL = runMedian(1, DEFAULT_SIZE, frontLeftValues, frontRightValues, DEFAULT_SIZE, default_device2, context2, program2);
    cout << "execution time: "<<execTimePOCL<<"s"<<endl;

    return 0;
}
