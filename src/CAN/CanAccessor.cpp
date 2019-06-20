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

CanErrorCode CanAccessor::didErrorOccur() {
    return NoError;
}

std::vector<u_int32_t> CanAccessor::getData() {
    //Thread.join();
    std::vector<u_int32_t> data;
    for(int i = 0; i<ElementCount; i++){
        data.push_back(Data[i]);
    }
    return data;
}

void CanAccessor::init() {
    Single = c3can_single_init("can0");
//    C3CAN_CHECK_ERR(Single, exit, -1);
    c3can_single_filter_add(Single, IdCan, (C3CAN_SINGLE_FILTER_OPTS) 0);

}

void CanAccessor::collectData() {
    struct timeval timestamp;
    for(int i = 0; i<ElementCount; i++){
        c3can_single_recv(Single, &Message, &timestamp);
//        C3CAN_CHECK_ERR(Single, exit, -1);
        Data[i] = U32_DATA(c3can_message_get_payload(&Message));
    }
}

void CanAccessor::startCollectingData() {

    //Thread=std::thread([=](){collectData();});
}
