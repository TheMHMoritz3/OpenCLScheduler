//
// Created by moritz on 17.06.19.
//

#include "CanAccessor.h"

#define U32_DATA(p) (*(uint32_t*)(p))

using namespace CAN;

CanAccessor::CanAccessor(CanID id, int elementCount) {
    IdCan = id;
    ElementCount=elementCount;
    init();
}

int CanAccessor::didErrorOccur() {
    return ErrorCode;
}

std::vector<uint32_t> CanAccessor::getData() {
    Thread.join();
	std::vector<uint32_t> data;
#ifndef _WINDOWS
    for(int i = 0; i<ElementCount; i++){
        data.push_back(Data[i]);
    }
#endif
    return data;
}

void CanAccessor::init() {
#ifndef _WINDOWS
    Single = c3can_single_init("can0");
    ErrorCode = c3can_single_get_error(Single)->code;
    c3can_single_filter_add(Single, IdCan, (C3CAN_SINGLE_FILTER_OPTS) 0);
#endif
}

void CanAccessor::collectData() {
#ifndef _WINDOWS
    struct timeval timestamp;
#endif
    for(int i = 0; i<ElementCount; i++){
#ifndef _WINDOWS
        c3can_single_recv(Single, &Message, &timestamp);
        ErrorCode = c3can_single_get_error(Single)->code;
        Data[i] = U32_DATA(c3can_message_get_payload(&Message));
#endif
    }
}

void CanAccessor::startCollectingData() {

    Thread=std::thread([=] {collectData();});
}
