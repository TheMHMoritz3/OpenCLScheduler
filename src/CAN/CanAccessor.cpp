//
// Created by moritz on 17.06.19.
//

#include "CanAccessor.h"
#include <iostream>
#include <exception>

#define U32_DATA(p) (*(uint32_t*)(p))

using namespace CAN;

CanAccessor::CanAccessor(CanID id, int elementCount) {
    IdCan = id;
    ElementCount=elementCount;
    init();
    Data = new uint32_t[ElementCount];
}

int CanAccessor::didErrorOccur() {
    return ErrorCode;
}

std::vector<uint32_t*> CanAccessor::getData() {
    Thread.join();
	std::vector<uint32_t*> data;
#ifndef _WINDOWS
    for(int i = 0; i<ElementCount; i++){
        data.push_back(&Data[i]);
    }
#endif
    return data;
}

void CanAccessor::init() {
#ifndef _WINDOWS
    Single = c3can_single_init("can0");
    ErrorCode = c3can_single_get_error(Single)->code;
    std::cout<<"Can Init ErrorCode "<<ErrorCode<<std::endl;
    c3can_single_filter_add(Single, IdCan, (C3CAN_SINGLE_FILTER_OPTS) 0);
    ErrorCode = c3can_single_get_error(Single)->code;
    std::cout<<"Can set Filter ErrorCode "<<ErrorCode<<std::endl;
#endif
}

void CanAccessor::collectData() {
#ifndef _WINDOWS
    struct timeval timestamp;
#endif
    for(int i = 0; i<ElementCount; i++){
#ifndef _WINDOWS
        try {
            c3can_message canMessage;
            c3can_single_recv(Single, &canMessage, &timestamp);
            ErrorCode = c3can_single_get_error(Single)->code;
            std::cout<<"Red Data ErrorCode "<<ErrorCode<<std::endl;
            Data[i] = U32_DATA(c3can_message_get_payload(&canMessage));
        } catch(std::exception ex){
            std::cout<<"Exception Occured"<<ex.what()<<std::endl;
            std::cout<<"Error Code"<<ErrorCode<<std::endl;
        } catch(...){
            std::cout<<"Error Code"<<ErrorCode<<std::endl;
        }
#endif
    }
}

void CanAccessor::startCollectingData() {

    Thread=std::thread([=] {collectData();});
    //collectData();
}
